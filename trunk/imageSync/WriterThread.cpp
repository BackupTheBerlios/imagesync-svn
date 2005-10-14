//////////////////////////////////////////////////////////////////////////////////////////
// 
//  Copyright (c) 2005 by Edmund Bayerle.
// 
//  Permission to copy, use and modify the software for any use 
//  (commercial and non-commercial) is hereby granted without fee, 
//  provided that the above copyright notice appears in all copies and 
//  that both that copyright notice and this permission notice appear 
//  in supporting documentation. Edmund Bayerle makes no representations 
//  about the suitability of this software for any purpose. It is 
//  provided "as is" without expressed or implied warranty. 
// 
//  $Id: WriterThread.cpp 25 2005-10-13 22:50:58Z ed $
// 

#include "WriterThread.h"
#include "CiffParser.h"
#include <QTime>
#include <QHash>
#include <QDir>
#include <QMutex>
#include <QTextStream>
#include <QFile>
#include <QFlags>
#include <QtDebug>

WriterThread::WriterThread( const JobQueueSynchronizedPtr & outgoingJobs )
      : outgoingJobs_( outgoingJobs ), closeThread_( false )
{
}

///////////////////////////////////////////////////////////////////////////////////////

void WriterThread::run()
{
   while( !closeThread_ ) {
      JobPtr job = outgoingJobs_->dequeueWait();
      StringHash recordParameters;
      // recordParameters.insert();
      
      if( !closeThread_ && !!job ) {
         qDebug() << "Processing file " << job->sourceFileName();

         // first scan the ciff records so that the gathered information could be used in naming the files
         CiffParser ciffParser( job->rawFileMap() );
         ciffParser.parse();
         QList< CiffRecordPtr > ciffRecords = ciffParser.ciffRecords();
         qDebug( "Total %d records", ciffRecords.size() );
         int countRecords = 0;
         CiffRecordPtr embeddedJpegRecord;

         QString fakePattern = "";
         QString destinationFileName = makeDestinationPathName( job, recordParameters, fakePattern ) + makeDestinationFileName( job, recordParameters, fakePattern );
         
         // TODO XXX: write a log file with decoded CIFF Records for each raw file processed
         QFile ciffLogFile( destinationFileName + ".txt" );
         ciffLogFile.open( QFile::WriteOnly );
         QTextStream ciffLog( &ciffLogFile );
         for( QList< CiffRecordPtr >::iterator it =  ciffRecords.begin(); it != ciffRecords.end(); ++it ) {
            CiffRecordPtr record = *it;
            ciffLog << "record " << countRecords++ ;
            logCiffRecord( ciffLog, record );
            if( record->tagId_ == 0x2007 ) {
               embeddedJpegRecord = record;
            }
         }
         ciffLogFile.close();
         
         if( !!embeddedJpegRecord ) {
            qDebug( "Found embedded Jpeg" );
            QFile jpegFile( destinationFileName + ".jpg" );
            jpegFile.open( QFile::WriteOnly );
            jpegFile.write( job->rawFileMap().mid( embeddedJpegRecord->absoluteStartOffset_, embeddedJpegRecord->length_ ) );
         }

         // write the raw file
         QFile destFile( destinationFileName + ".crw" );
         destFile.open( QFile::WriteOnly );
         destFile.write( job->rawFileMap() );
         destFile.close();
         
         oneJobFinished();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////////

void WriterThread::logCiffRecord( QTextStream & ciffLog, const CiffRecordPtr & record )
{
   ciffLog << " tagId: " << record->tagId_ << " length: " << dec << record->length_;
   ciffLog << endl;
}

///////////////////////////////////////////////////////////////////////////////////////

void WriterThread::makePath( QString path )
{
   if( existingDirectories_.contains( path ) ) return;
   QDir rootPath = QDir::rootPath();
   rootPath.mkpath( path );
   existingDirectories_.insert( path );
}

///////////////////////////////////////////////////////////////////////////////////////

QString WriterThread::makeDestinationPathName( const JobPtr & job, const StringHash & /* ciffRecordParameters */,
                                               const QString & /* pattern */ ) const
{
   return job->destinationBasePath();
}

///////////////////////////////////////////////////////////////////////////////////////

QString WriterThread::makeDestinationFileName( const JobPtr & job, const StringHash & /* ciffRecordParameters */,
                                               const QString & /* pattern */ ) const
{
   // TODO : remove the crw from the destinationFileName
   return job->destinationFileName();
}

///////////////////////////////////////////////////////////////////////////////////////

void WriterThread::quitRequested()
{
   closeThread_ = true;
}

