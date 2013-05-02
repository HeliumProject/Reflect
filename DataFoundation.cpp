#include "ReflectPch.h"
#include "DataFoundation.h"

using namespace Helium;
using namespace Helium::Reflect;

StringData::StringData() :
ScalarData( sizeof( String ), ScalarTypes::String )
{
}

void Helium::Reflect::StringData::Construct( DataPointer pointer )
{
	new ( pointer.m_Address ) String;
}

void Helium::Reflect::StringData::Destruct( DataPointer pointer )
{
	static_cast< String* >( pointer.m_Address )->~String();
}

void StringData::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	String& right = src.As<String>();
	String& left = dest.As<String>();
	left = right;
}

bool StringData::Equals( DataPointer a, DataPointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	String& right = a.As<String>();
	String& left = b.As<String>();
	return left == right;
}

void StringData::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	string = pointer.As<String>();
}

void StringData::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	pointer.As<String>() = string;
}

void StringData::Accept( DataPointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}
