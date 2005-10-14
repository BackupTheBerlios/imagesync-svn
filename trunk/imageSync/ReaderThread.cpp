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
//  $Id: ReaderThread.cpp 25 2005-10-13 22:50:58Z ed $
// 

#include "ReaderThread.h"
#include <QTime>
#include <QMutex>
#include <QTextStream>
#include <QFile>
#include <QFlags>
#include <QtDebug>

ReaderThread::ReaderThread( const JobQueueSynchronizedPtr & incommingJobs, const JobQueueSynchronizedPtr & outgoingJobs )
      : incommingJobs_( incommingJobs ), outgoingJobs_( outgoingJobs ), closeThread_( false )
{
}


void ReaderThread::run()
{
   sumElapsedTime_ = 0;
   sumFileSize_ = 0;
   int fileCounter = 0;
   while( !closeThread_ ) {
      JobPtr job = incommingJobs_->dequeueWait();
      if( !closeThread_ && !!job ) {
         qDebug( "start ReaderThread" );
         copyFile( job );
         outgoingJobs_->enqueueWait( job );
         ++fileCounter;
         signalStatus( fileCounter );
      }
   }
}

void ReaderThread::quitRequested()
{
   closeThread_ = true;
}

/// return number of bytes copied
int ReaderThread::copyFile( const JobPtr & job )
{
   QTime chronometer;
   reportOperation( "Copying " + job->sourceFileName() );
   chronometer.start();
   QString rawFileName = job->sourcePath() + "/" + job->sourceFileName();
   QFile srcFile( rawFileName );
   srcFile.open( QFile::ReadOnly );
   QByteArray rawBytes = srcFile.readAll();
   qDebug() << "Read" << rawFileName << rawBytes.size() << "bytes";
   job->setRawFileMap( rawBytes );
   sumFileSize_ += job->fileSize();
   sumElapsedTime_ += chronometer.elapsed();
   
   QString formattedThroughput = formatSpeed( sumFileSize_, sumElapsedTime_ );
   reportSpeed( formattedThroughput );
   return job->fileSize();
}


QString ReaderThread::formatSpeed( int throughput, int miliseconds )
{
   double speed = ( throughput * 1000.0 ) / ( miliseconds * 1024.0 * 1024.0 );
   return QString( "%1 Mb/s" ).arg( speed );
}
