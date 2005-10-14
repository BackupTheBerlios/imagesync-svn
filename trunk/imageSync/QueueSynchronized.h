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
//  $Id: QueueSynchronized.h 25 2005-10-13 22:50:58Z ed $
// 

#ifndef INC__IMAGESYNC__QUEUESYNCHRONIZED_H
#define INC__IMAGESYNC__QUEUESYNCHRONIZED_H

#include <boost/shared_ptr.hpp>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QBuffer>
#include <QByteArray>

#include "Job.h"

template < class T >
class QueueSynchronized: public QQueue<T>
{
public:
   typedef T ElementType;
   
   QueueSynchronized() : fullWatermark_( 1 ) { }

   void setFullWatermark( int watermark ) { fullWatermark_ = watermark; }

   
   ElementType dequeueWait()
   {
      QMutexLocker mutexLocker( &accessMutex_ );
      if( QQueue<T>::empty() ) {
         wait();
      }
      if( QQueue<T>::empty() ) {
         return T();
      }
      else {
         ElementType element( QQueue<T>::dequeue() );
         wakeOne();
         return element;
      }
   }

   
   void enqueueWait( const ElementType & element )
   {
      QMutexLocker mutexLocker( &accessMutex_ );
      if( QQueue<T>::size() > fullWatermark_ ) {
         wait();
      }
      QQueue<T>::enqueue( element );
      wakeOne();
   }

   
   void lock()    { accessMutex_.lock(); }
   void unlock()  { accessMutex_.unlock(); }
   void wait()    { accessCondition_.wait( &accessMutex_ ); }
   void wakeOne() { accessCondition_.wakeOne(); }
   void wakeAll() { accessCondition_.wakeAll(); }
      
   QMutex * mutex() { return &accessMutex_; }
   
private:
   QMutex                accessMutex_;
   QWaitCondition        accessCondition_;
   int                   fullWatermark_;
};

typedef QueueSynchronized< JobPtr > JobQueueSynchronized;
typedef boost::shared_ptr< JobQueueSynchronized > JobQueueSynchronizedPtr;

#endif // INC__IMAGESYNC__QUEUESYNCHRONIZED_H
