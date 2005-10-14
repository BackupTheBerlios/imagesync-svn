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
//  $Revision:  $
//  $Date:  $
//

#ifndef INC__IMAGESYNC__CIFFPARSER_H
#define INC__IMAGESYNC__CIFFPARSER_H

#include <QList>
#include <QIODevice>
#include <QByteArray>
#include <boost/shared_ptr.hpp>

class CiffRecord 
{
public:
   CiffRecord( const int tagId, const int absoluteStartOffset, const int length );
   
   int tagId_;
   int absoluteStartOffset_;
   int length_;
};

typedef boost::shared_ptr< CiffRecord > CiffRecordPtr;

class CiffParser;

class CiffDirectoryEntry;
typedef boost::shared_ptr< CiffDirectoryEntry > CiffDirectoryEntryPtr;


#define CIFF_DIRECTORY_DATA_LOCATION_VALUE_DATA  0
#define CIFF_DIRECTORY_DATA_LOCATION_DIRECTORY   1

#define CIFF_DIRECTORY_DATA_FORMAT_BYTE_ARRAY    0
#define CIFF_DIRECTORY_DATA_FORMAT_ASCII_ARRAY   1
#define CIFF_DIRECTORY_DATA_FORMAT_SHORT_ARRAY   2
#define CIFF_DIRECTORY_DATA_FORMAT_LONG_ARRAY    3
#define CIFF_DIRECTORY_DATA_FORMAT_MIXTURE       4
#define CIFF_DIRECTORY_DATA_FORMAT_DIRECTORY0    5
#define CIFF_DIRECTORY_DATA_FORMAT_DIRECTORY1    6
#define CIFF_DIRECTORY_DATA_FORMAT_UNKNOWN       7

class CiffDirectoryEntry 
{
public:
   static CiffDirectoryEntryPtr parse( int & readPos, const int absoluteStartOffset,
                                       const CiffParser & p );
   int tag_;
   int length_;
   int absoluteStartOffset_;
   int dataLocation_;
   int dataFormat_;
   int tagId_;
};


///////////////////////////////////////////////////////////////////////////////////////

class CiffParser
{
public:
   CiffParser( const QByteArray & ciffFile );
   int get2( int & pos ) const;
   int get4( int & pos ) const;
   QByteArray read( int & pos, int size ) const;
   void parse();
   void parseCiffDirectory( int absoluteOffset, int length );
   void parseRecords( int offset, int length );
   QList< CiffRecordPtr > ciffRecords() { return ciffRecords_; }
   
private:
   QByteArray rawFileMap_;
   bool   littleEndian_;
   QList< CiffRecordPtr > ciffRecords_;
   int rawMapCurrentPosition_;
};

#endif // INC__IMAGESYNC__CIFFPARSER_H
