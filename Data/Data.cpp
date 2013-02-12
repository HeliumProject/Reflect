#include "ReflectPch.h"
#include "Reflect/Data/Data.h"

#include "Platform/Exception.h"
#include "Foundation/TUID.h"
#include "Foundation/Numeric.h"
#include "Reflect/Data/DataDeduction.h"

REFLECT_DEFINE_ABSTRACT( Helium::Reflect::Data )

using namespace Helium;
using namespace Helium::Reflect;

Data::Data()
: m_Field( NULL )
{

}

Data::~Data()
{

}

void Data::ConnectField( void* instance, const Field* field, uintptr_t offsetInField )
{
    m_Instance = instance;
    m_Field = field;
    ConnectData( static_cast< char* >( m_Instance ) + m_Field->m_Offset + offsetInField ); 
}

void Data::Disconnect()
{
    m_Instance = (Object*)NULL;
    m_Field = NULL;
    ConnectData( NULL );
}

bool Data::ShouldSerialize()
{
    return true;
}

tostream& Data::operator>>(tostream& stream) const
{ 
    HELIUM_BREAK(); 
    return stream; 
}

tistream& Data::operator<<(tistream& stream)
{ 
    HELIUM_BREAK(); 
    return stream; 
}

void Data::Accept(Visitor& visitor)
{
    // by default, don't do anything as it will all have to be special cased in derived classes
}

// See also Data::Bind in .inl file
Helium::Reflect::DataPtr Helium::Reflect::Data::BindStructure( void* value, void* instance, const Field* field )
{
    DataPtr ser = AssertCast<Data>( StructureData::CreateObject() );

    if (ser.ReferencesObject())
    {
        ser->m_Instance = instance;
        ser->m_Field = field;
        ser->ConnectData( value );
    }

    return ser;
}
