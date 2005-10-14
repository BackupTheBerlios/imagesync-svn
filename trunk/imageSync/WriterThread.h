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
//  $Id: WriterThread.h 25 2005-10-13 22:50:58Z ed $
// 

#ifndef INC__IMAGESYNC__WRITERTHREAD_H
#define INC__IMAGESYNC__WRITERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QSet>
#include <QTextStream>
#include "CiffParser.h"
#include "QueueSynchronized.h"

typedef QHash< QString, QString > StringHash;

class WriterThread: public QThread
{
   Q_OBJECT

public:
   WriterThread( const JobQueueSynchronizedPtr & outgoingJobs  );
   
protected:
   void run();

signals:
   void operationCanceled();
   void oneJobFinished();

public slots:
   void quitRequested();

private:
   void logCiffRecord( QTextStream & ciffLog, const CiffRecordPtr & record );
   void makePath( QString path );
   QString makeDestinationFileName( const JobPtr & job, const StringHash & ciffRecordParameters,
                                    const QString & pattern ) const;
   QString makeDestinationPathName( const JobPtr & job, const StringHash & ciffRecordParameters,
                                    const QString & pattern ) const;

private:
   QSet< QString > existingDirectories_;
   JobQueueSynchronizedPtr outgoingJobs_;
   bool closeThread_;
};

#endif // INC__IMAGESYNC__WRITERTHREAD_H
