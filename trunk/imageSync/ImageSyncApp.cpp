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
//  $Id: ImageSyncApp.cpp 25 2005-10-13 22:50:58Z ed $
// 

#include "ImageSyncApp.h"
#include <QMutexLocker>
#include <QShortcut>
#include <QFileDialog>

#define SOURCE_DIRECTORY "/home/ed/digifoto/microdrive"
#define DESTINATION_DIRECTORY "/home/ed/digifoto/10d"
#define CONFIGURATION_FILE "imageSync.config"

///////////////////////////////////////////////////////////////////////////////////////

ImageSyncMainWindow::ImageSyncMainWindow()
      : QMainWindow()
{
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncMainWindow::closeEvent( QCloseEvent * event )
{
   closeRequested();
   QMainWindow::closeEvent( event );
}

///////////////////////////////////////////////////////////////////////////////////////

ImageSyncApp::ImageSyncApp( int & argc, char ** argv )
      : QApplication( argc, argv ),
        mainWindow_( new ImageSyncMainWindow ), mainWindowContent_( new Ui::MainWindow ),
        incommingJobs_( new JobQueueSynchronized ), outgoingJobs_( new JobQueueSynchronized ),
        countJobsRequested_( 0 ), countJobsDone_( 0 ), operationCanceled_( false )
{
   incommingJobs_->setFullWatermark( 2000 );
   outgoingJobs_->setFullWatermark( 1 );
   initializeMainWindow();
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::initializeMainWindow()
{
   configurationFile_ = boost::shared_ptr< ConfigurationFile > ( new ConfigurationFile( CONFIGURATION_FILE ) );

   readerThread_.reset( new ReaderThread( incommingJobs_, outgoingJobs_ ) );
   writerThread_.reset( new WriterThread( outgoingJobs_ ) );

   mainWindowContent_->setupUi( mainWindow_.get() );
   mainWindow_->setWindowTitle( "imageSync - E.B." );
   mainWindowContent_->downloadProgress->setMinimum( 0 );
   mainWindowContent_->downloadProgress->setMaximum( 0 );
   mainWindowContent_->downloadProgress->setValue( 0 );
   mainWindowContent_->buttonCancel->setEnabled( false );
   mainWindow_->show();
   
   new QShortcut( Qt::CTRL + Qt::Key_Q, mainWindow_.get(), SLOT( close() ) );
   QObject::connect( mainWindowContent_->buttonCancel, SIGNAL( clicked() ),
                     this, SLOT( cancelRequested() ) );
   QObject::connect( mainWindowContent_->buttonGo, SIGNAL( clicked() ),
                     this, SLOT( startConversion() ) );
   QObject::connect( mainWindowContent_->buttonBrowseSource, SIGNAL( clicked() ),
                     this, SLOT( browseSource() ) );
   QObject::connect( mainWindowContent_->buttonBrowseDestination, SIGNAL( clicked() ),
                     this, SLOT( browseDestination() ) );
   QObject::connect( mainWindowContent_->tabWidget, SIGNAL( currentChanged( int ) ),
                     this, SLOT( currentTabChanged( int ) ) );
   QObject::connect( mainWindow_.get(), SIGNAL( closeRequested() ),
                     this, SLOT( mainWindowCloseRequested() ) );
   QObject::connect( readerThread_.get(), SIGNAL( reportOperation( QString ) ),
                     mainWindowContent_->statusLine, SLOT( setText( QString ) ) );
   QObject::connect( readerThread_.get(), SIGNAL( reportSpeed( QString ) ),
                     mainWindowContent_->speedLine, SLOT( setText( QString ) ) );
   QObject::connect( writerThread_.get(), SIGNAL( oneJobFinished() ),
                     this, SLOT( writerFinishedOneJob() ) );

   populateGuiFromConfiguration();
   populateTableWidget();
   readerThread_->start();
   writerThread_->start();
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::browseSource()
{
   QString s = QFileDialog::getExistingDirectory(
      NULL,
      "Choose a directory",
      mainWindowContent_->textEditSource->text(),
      QFileDialog::DontResolveSymlinks
   );
   if( !s.isEmpty() ) {
      mainWindowContent_->textEditSource->setText( s );
      configurationFile_->setValue( "sourceDirectory", s );
   }
}
   
///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::browseDestination()
{
   QString s = QFileDialog::getExistingDirectory(
      NULL,
      "Choose a directory",
      mainWindowContent_->textEditDestination->text(),
      QFileDialog::DontResolveSymlinks
   );
   if( !s.isEmpty() ) {
      mainWindowContent_->textEditDestination->setText( s );
      configurationFile_->setValue( "destinationDirectory", s );
   }
   
}
   
///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::currentTabChanged( int currentTabIndex )
{
   if( currentTabIndex == 0 ) {
      // we are on the process page, so copy all the relevant fields
      // into the coinfigurationFile
      populateConfigurationFormGui();
      populateTableWidget();
   }
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::populateConfigurationFormGui()
{
   configurationFile_->setValue( "destinationDirectory",
                                 mainWindowContent_->textEditDestination->text() );
   configurationFile_->setValue( "sourceDirectory",
                                 mainWindowContent_->textEditSource->text() );
}
   
///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::populateGuiFromConfiguration()
{
   PropertyPtr property = configurationFile_->value( "destinationDirectory", DESTINATION_DIRECTORY );
   mainWindowContent_->textEditDestination->setText( !property ? "" : property->asString() );
   property = configurationFile_->value( "sourceDirectory", SOURCE_DIRECTORY );
   mainWindowContent_->textEditSource->setText( !property ? "" : property->asString() );
   
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::populateTableWidget()
{
   QFileInfoList allFiles;
   recursiveBuildFileInfoList( allFiles,
                               configurationFile_->value( "sourceDirectory", SOURCE_DIRECTORY )->asString() );
   extractFileNames( allFiles, mainWindowContent_->filesView );
}


///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::startConversion()
{
   QFileInfoList allFiles;
   recursiveBuildFileInfoList( allFiles,
                               configurationFile_->value( "sourceDirectory", SOURCE_DIRECTORY )->asString() );
   incommingJobs_->lock();
   countJobsRequested_ = incommingJobs_->size() + allFiles.size();
   extractFileNames( allFiles, incommingJobs_,
                     configurationFile_->value( "destinationDirectory", DESTINATION_DIRECTORY )->asString() );
   countJobsDone_ = 0;
   incommingJobs_->wakeOne();
   incommingJobs_->unlock();
   qDebug( "begin startConversion with %d jobs", countJobsRequested_ );
   mainWindowContent_->downloadProgress->setMaximum( countJobsRequested_ );
   mainWindowContent_->buttonCancel->setEnabled( true );
   mainWindowContent_->buttonGo->setEnabled( false );
   operationCanceled_ = false;
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::writerFinishedOneJob()
{
   countJobsDone_++;
   mainWindowContent_->downloadProgress->setValue( countJobsDone_ );
   if( countJobsDone_ == countJobsRequested_ ) {
      mainWindowContent_->buttonCancel->setEnabled( false );
      mainWindowContent_->buttonGo->setEnabled( true );
      mainWindowContent_->statusLine->setText( operationCanceled_ ? "Job canceled!" : "Job finished!" );
   }
   qDebug( "countJobsDone_: %d", countJobsDone_ );
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::mainWindowCloseRequested()
{
   cancelRequested();
   readerThread_->quitRequested();
   writerThread_->quitRequested();
   incommingJobs_->wakeAll();
   outgoingJobs_->wakeAll();
   readerThread_->wait();
   writerThread_->wait();
   qDebug( "mainWindowCloseRequested()" );
   quit();
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::cancelRequested()
{
   mainWindowContent_->statusLine->setText( "Canceling operation ...  " );
   {
      QMutexLocker locker( incommingJobs_->mutex() );
      countJobsRequested_ -= incommingJobs_->size();
      incommingJobs_->clear();
      operationCanceled_ = true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::recursiveBuildFileInfoList( QFileInfoList & allFiles, const QString & directoryName )
{
   QDir::QDir directoryContent( directoryName );
   QString absolutePath = QDir::cleanPath( directoryContent.absolutePath() ) + "/";
   QStringList subdirs = directoryContent.entryList( QDir::Dirs );
   for( QStringList::iterator it = subdirs.begin(); it != subdirs.end(); ++it ) {
      QString currentDir = *it;
      if( currentDir == "." || currentDir == ".." ) continue;
      QString path = absolutePath + currentDir;
      recursiveBuildFileInfoList( allFiles, path );
   }

   QFileInfoList files = directoryContent.entryInfoList( QDir::Files );
   for( QFileInfoList::iterator it = files.begin(); it != files.end(); ++it ) {
      QFileInfo currentFile = *it;
      if( currentFile.fileName().endsWith( "crw" ) ) {
         allFiles.append( currentFile );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::extractFileNames( const QFileInfoList & allFiles, QTableWidget * filesView )
{
   filesView->setRowCount( allFiles.size() );
   filesView->setColumnCount( 2 );
   filesView->verticalHeader()->hide();
   QHeaderView * horizontalHeader = filesView->horizontalHeader();
   horizontalHeader->setResizeMode( 0, QHeaderView::Stretch );
   horizontalHeader->setResizeMode( 1, QHeaderView::Custom );
   QStringList headerLabels;
   headerLabels << "Name" << "Date";
   filesView->setHorizontalHeaderLabels( headerLabels );

   int rowCounter = 0;
   for( QFileInfoList::const_iterator it = allFiles.begin(); it != allFiles.end(); ++it ) {
      QFileInfo fileInfo = *it;
      filesView->setItem( rowCounter, 0, new QTableWidgetItem( fileInfo.fileName() ) );
      filesView->setItem( rowCounter, 1, new QTableWidgetItem( fileInfo.lastModified().toString() ) );
      filesView->resizeRowToContents( rowCounter );
      rowCounter++;
   }
}

///////////////////////////////////////////////////////////////////////////////////////

void ImageSyncApp::extractFileNames( const QFileInfoList & allFiles, JobQueueSynchronizedPtr & jobs,
                                     QString destinationDirectory )
{
   for( QFileInfoList::const_iterator it = allFiles.begin(); it != allFiles.end(); ++it ) {
      QFileInfo fileInfo = *it;
      JobPtr job( new Job( fileInfo.absolutePath(), fileInfo.fileName(), destinationDirectory, fileInfo.baseName(), fileInfo.size() ) );
      jobs->enqueue( job );
   }
}
