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
//  $Id: ReaderThread.h 23 2005-10-04 20:44:52Z ed $
// 

#ifndef INC__IMAGESYNC__READERTHREAD_H
#define INC__IMAGESYNC__READERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include "QueueSynchronized.h"

class ReaderThread: public QThread
{
   Q_OBJECT

public:
   ReaderThread( const JobQueueSynchronizedPtr & incommingJobs, const JobQueueSynchronizedPtr & outgoingJobs  );
   
protected:
   void run();

signals:
   void signalStatus( int status );
   void reportOperation( QString operation );
   void reportSpeed( QString speed );
   void operationCanceled();

public slots:
   void quitRequested();

private:
   int copyFile( const JobPtr & job );
   QString formatSpeed( int throughput, int miliseconds );
   
private:
   JobQueueSynchronizedPtr incommingJobs_;
   JobQueueSynchronizedPtr outgoingJobs_;
   int sumFileSize_;
   int sumElapsedTime_;
   bool closeThread_;
};

#endif // INC__IMAGESYNC__READERTHREAD_H
