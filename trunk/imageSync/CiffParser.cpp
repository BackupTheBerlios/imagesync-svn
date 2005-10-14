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
//  $Id: CiffParser.cpp 25 2005-10-13 22:50:58Z ed $
// 

#include "CiffParser.h"
#include <QtDebug>


// NEEDS FIX A: other modell of file, QIODevice
// use QByteArray and int
// abstract the CiffRecord and handle it better ( maybe collect them in a QList )
// read the ciff documentation and define constants for record types

///////////////////////////////////////////////////////////////////////////////////////

CiffParser::CiffParser( const QByteArray & ciffFileMap )
      : rawFileMap_( ciffFileMap ), littleEndian_( true )
{
}

///////////////////////////////////////////////////////////////////////////////////////

int CiffParser::get2( int & pos ) const
{
   char c1 = rawFileMap_[ pos++ ];
   char c2 = rawFileMap_[ pos++ ];
   
   return littleEndian_ ?
      ( (  c1 & 0xff ) + ( (  c2 & 0xff ) << 8 ) ) :
      ( ( ( c1 & 0xff ) << 8 )  + ( c2 & 0xff ) );
}

///////////////////////////////////////////////////////////////////////////////////////

int CiffParser::get4( int & pos ) const
{
   return littleEndian_ ?
      ( get2( pos ) + ( get2( pos ) << 16 ) ) :
      ( ( get2( pos ) << 16 )  + get2( pos ) );
}

///////////////////////////////////////////////////////////////////////////////////////

QByteArray CiffParser::read( int & pos, int size ) const
{
   int startPos = pos;
   pos += size;
   return rawFileMap_.mid( startPos, size );
}

///////////////////////////////////////////////////////////////////////////////////////

void CiffParser::parse()
{
   int readPos = 0;
   int order = get2( readPos );
   littleEndian_ = order == 0x4949;
   int headerLenght = get4( readPos );
   QByteArray head = read( readPos, 32 );
   int fsize = rawFileMap_.size();
   // qDebug() << "fsize:" << fsize << "headerLenght:" << headerLenght << head.indexOf( "HEAPCCDR" );
   if( head.indexOf( "HEAPCCDR" ) == 0 ) {
      // qDebug() << "CRW file identified";
      parseCiffDirectory( headerLenght, fsize - headerLenght );
   }
}

///////////////////////////////////////////////////////////////////////////////////////

void CiffParser::parseCiffDirectory( int absoluteStartOffset, int length )
{
   QList< CiffDirectoryEntryPtr > directoryEntries;
   int startOfDirectory = absoluteStartOffset + length - 4;
   int valueDataSize = get4( startOfDirectory );
   qDebug( "parseCiffDirectory absoluteStartOffset: 0x%x length: 0x%x valueDataSize: 0x%x",
           absoluteStartOffset, length, valueDataSize );
   int directoryEntriesPosition = absoluteStartOffset + valueDataSize;
   int directoryEntryCount = get2( directoryEntriesPosition );
   qDebug( "directoryEntryCount %d directory entries found", directoryEntryCount );
   for( int i = 0; i < directoryEntryCount; ++i ) {
      // parse the directory entries
      directoryEntries.append( CiffDirectoryEntry::parse( directoryEntriesPosition, absoluteStartOffset, *this ) );
   }
   
   for( QList< CiffDirectoryEntryPtr >::iterator it = directoryEntries.begin(); it != directoryEntries.end(); ++it ) {
      CiffDirectoryEntryPtr directory = *it;
      if( ( directory->dataFormat_ == CIFF_DIRECTORY_DATA_FORMAT_DIRECTORY0 ) ||
          ( directory->dataFormat_ == CIFF_DIRECTORY_DATA_FORMAT_DIRECTORY1 ) ) {
         parseCiffDirectory( directory->absoluteStartOffset_, directory->length_ );
      }
      else if( directory->dataLocation_ == CIFF_DIRECTORY_DATA_LOCATION_VALUE_DATA ) {
         CiffRecordPtr ciffRecord( new CiffRecord( directory->tagId_, directory->absoluteStartOffset_, directory->length_ ) );
         ciffRecords_.append( ciffRecord );
      }
      else if( directory->dataLocation_ == CIFF_DIRECTORY_DATA_LOCATION_DIRECTORY ) {
         /// TODO XXX: // there is no way to reconstruct at this time the content of the inline directory entry 
         // rawPos = directory->absoluteStartOffset_;
         // CiffRecordPtr ciffRecord( new CiffRecord( directory->tagId_, read( rawPos, directory->length_ ) ) );
         // ciffRecords_.append( ciffRecord );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////////

CiffDirectoryEntryPtr CiffDirectoryEntry::parse( int & readPosition,
                                                 const int absoluteStartOffset,
                                                 const CiffParser & p )
{
   CiffDirectoryEntryPtr dirEntry( new CiffDirectoryEntry );
   dirEntry->tag_ = p.get2( readPosition );
   dirEntry->length_ = p.get4( readPosition );
   dirEntry->absoluteStartOffset_ = absoluteStartOffset + p.get4( readPosition );
   dirEntry->dataLocation_ = ( dirEntry->tag_ & 0xc000 ) >> 14;
   dirEntry->dataFormat_ = ( dirEntry->tag_ & 0x3800 ) >> 11;
   dirEntry->tagId_ = ( dirEntry->tag_ & 0x3fff );
   return dirEntry;
}

///////////////////////////////////////////////////////////////////////////////////////

CiffRecord::CiffRecord( int tagId, int absoluteStartOffset, int length )
      : tagId_( tagId ), absoluteStartOffset_( absoluteStartOffset ), length_( length )
{
}
