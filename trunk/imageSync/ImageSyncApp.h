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
//  $Id: ImageSyncApp.h 25 2005-10-13 22:50:58Z ed $
// 

#ifndef INC__IMAGESYNC__IMAGESYNCAPP_H
#define INC__IMAGESYNC__IMAGESYNCAPP_H

#include <QApplication>
#include <QMainWindow>
#include <QThread>
#include <boost/shared_ptr.hpp>
#include "ConfigurationFile.h"

#include <QtGui>
#include <QtDebug>
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QDir>
#include <QTableWidget>
#include <QCloseEvent>
#include <QMainWindow>
#include <iostream>

#include "ReaderThread.h"
#include "WriterThread.h"
#include "ConfigurationFile.h"
#include "ImageSyncGui.h"
#include "QueueSynchronized.h"

///////////////////////////////////////////////////////////////////////////////////////

class ImageSyncMainWindow : public QMainWindow
{
   Q_OBJECT

public:
   ImageSyncMainWindow();
   void closeEvent( QCloseEvent * event );

signals:
   void closeRequested();
};

///////////////////////////////////////////////////////////////////////////////////////

class ImageSyncApp: public QApplication
{
   Q_OBJECT
   
public:
   ImageSyncApp( int & argc, char ** argv );
   void initializeMainWindow();

   typedef boost::shared_ptr< QBuffer > QBufferPtr;

public slots:
   void startConversion();
   void currentTabChanged( int );
   void browseSource();
   void browseDestination();
   void cancelRequested();
   void mainWindowCloseRequested();
   void writerFinishedOneJob();
   void populateConfigurationFormGui();
   void populateGuiFromConfiguration();
   void populateTableWidget();
   
protected:
   void recursiveBuildFileInfoList( QFileInfoList & allFiles, const QString & directoryName );
   void extractFileNames( const QFileInfoList & allFiles, QTableWidget * filesView );
   void extractFileNames( const QFileInfoList & allFiles, JobQueueSynchronizedPtr & jobs, QString destinationDirectory );
   
private:
   boost::shared_ptr< ImageSyncMainWindow > mainWindow_;
   boost::shared_ptr< Ui::MainWindow >     mainWindowContent_;
   boost::shared_ptr< ReaderThread >       readerThread_;
   boost::shared_ptr< WriterThread >       writerThread_;
   boost::shared_ptr< ConfigurationFile >  configurationFile_;
   JobQueueSynchronizedPtr                 incommingJobs_;
   JobQueueSynchronizedPtr                 outgoingJobs_;
   int                                     countJobsRequested_;
   int                                     countJobsDone_;
   bool                                    operationCanceled_;
};

///////////////////////////////////////////////////////////////////////////////////////

#endif // INC__IMAGESYNC__IMAGESYNCAPP_H
