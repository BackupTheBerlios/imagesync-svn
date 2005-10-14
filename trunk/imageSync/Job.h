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
//  $Id$
// 

#ifndef INC__IMAGESYNC__JOB_H
#define INC__IMAGESYNC__JOB_H

#include <boost/shared_ptr.hpp>
#include <QByteArray>

#define DIR_SEPPARATOR QChar( '/' )
#define FILE_TYPE_SEPPARATOR QChar( '.' )

typedef boost::shared_ptr< QByteArray > QByteArrayPtr; 

class Job
{
public:
   Job() {}
   Job( QString srcPath, QString srcFile, QString dstPath, QString dstFile, int fileSize )
         : sourcePath_( srcPath ), sourceFileName_( srcFile ),
           destinationBasePath_( dstPath ), destinationFileName_( dstFile ),
           fileSize_( fileSize )
   {
      if( sourcePath_.at( sourcePath_.size() - 1 ) != DIR_SEPPARATOR ) {
         sourcePath_.append( DIR_SEPPARATOR );
      }
      if( destinationBasePath_.at( destinationBasePath_.size() - 1 ) != DIR_SEPPARATOR ) {
         destinationBasePath_.append( DIR_SEPPARATOR );
      }
   }
   
   const QString sourcePath()             const { return sourcePath_; }
   const QString sourceFileName()         const { return sourceFileName_; } 
   const QString destinationBasePath()    const { return destinationBasePath_; }
   const QString destinationFileName()    const { return destinationFileName_; }
   const int     fileSize()               const { return fileSize_; }
   
   void  setRawFileMap( QByteArray & rawData )
   {
      rawFileMap_ = rawData;
   }
   
   QByteArray & rawFileMap() { return rawFileMap_; }
   
private:
   QString sourcePath_;
   QString sourceFileName_;
   QString destinationBasePath_;
   QString destinationFileName_;
   QByteArray rawFileMap_;
   int fileSize_;
};

typedef boost::shared_ptr< Job > JobPtr;


#endif // INC__IMAGESYNC__JOB_H
