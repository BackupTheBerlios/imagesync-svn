#ifndef CONFIGURATION_FILE_H
#define CONFIGURATION_FILE_H

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QList>
#include <QHash>
#include <boost/shared_ptr.hpp>

///////////////////////////////////////////////////////////////////////////////////////

class Property
{
public:
   QString asString() const;
   bool asBoolean() const;
   Property( const QString & name, const QString & val );

private:
   QString textValue_;
   QString name_;
};

typedef boost::shared_ptr<Property> PropertyPtr;

///////////////////////////////////////////////////////////////////////////////////////

class ConfigurationFile
{
public:
   ConfigurationFile( const QString & fileName );
   PropertyPtr value( const QString & propertyName, const QString & defaultValue ) const;
   PropertyPtr setValue( const QString & propertyName, const QString &  propertyValue );
   void readFromFile( const QString & fileName );

private:
   QHash< QString, PropertyPtr > propertiesMap_;
};

#endif
