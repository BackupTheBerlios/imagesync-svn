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
//  $Id: ConfigurationFile.cpp 25 2005-10-13 22:50:58Z ed $
// 

#include "ConfigurationFile.h"
#include <QtDebug>
#include <QDomDocument>
#include <QDomNodeList>
#include <QFile>
#include <iostream>
#include <QMessageBox>
#include <QtDebug>

#define PROPERTY_TAG "p"

///////////////////////////////////////////////////////////////////////////////////////

Property::Property( const QString & name, const QString & value  ) 
      : textValue_( value ), name_( name )
{
}

///////////////////////////////////////////////////////////////////////////////////////

QString Property::asString() const
{
   return textValue_;
}

///////////////////////////////////////////////////////////////////////////////////////

bool Property::asBoolean() const
{
   // TODO: implement the right thing
   return false;
}

///////////////////////////////////////////////////////////////////////////////////////

ConfigurationFile::ConfigurationFile( const QString & fileName )
{
   readFromFile( fileName );
}

///////////////////////////////////////////////////////////////////////////////////////

void ConfigurationFile::readFromFile( const QString & fileName )
{
   QDomDocument doc( "mydocument" );
    QFile file( fileName );
    if( !file.open( QIODevice::ReadOnly ) ) {
       qDebug() << "Could not open file " << fileName;
       return;
    }
    if( !doc.setContent( &file ) ){
        file.close();
        qDebug() << "Inconsistent XML file  " << fileName;
        return;
    }
    file.close();

    QDomNodeList allElements = doc.elementsByTagName( PROPERTY_TAG );
    int elementsCount = allElements.count();
    for( int i = 0; i < elementsCount; i++ ) {
       QDomElement e = allElements.item( i ).toElement();
       QString propertyName = e.attribute( "name" );
       QString value = e.text();
       PropertyPtr p( new Property( propertyName, value ) );
       propertiesMap_.insert( propertyName, p );
       qDebug() << propertyName << value;
    }
}

///////////////////////////////////////////////////////////////////////////////////////

PropertyPtr ConfigurationFile::value( const QString & propertyName, const QString & defaultValue ) const
{
   PropertyPtr p = propertiesMap_.value( propertyName );
   if( !p ) {
      PropertyPtr p1( new Property( propertyName, defaultValue ) );
      return p1;
   }
   else {
      return p;
   }
}

///////////////////////////////////////////////////////////////////////////////////////

PropertyPtr ConfigurationFile::setValue( const QString & propertyName,
                                         const QString &  propertyValue ) 
{
   PropertyPtr p( new Property( propertyName, propertyValue ) );
   propertiesMap_.insert( propertyName, p );
   return p;
}
