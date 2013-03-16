#include "ReflectPch.h"
#include "DataFoundation.h"

using namespace Helium;
using namespace Helium::Reflect;

bool StringData::Copy( DataInstance src, DataInstance dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	String* right = src.GetAddress<String>();
	String* left = dest.GetAddress<String>();
	*left = *right;
	return true;
}

bool StringData::Equals( DataInstance a, DataInstance b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	String* right = a.GetAddress<String>();
	String* left = b.GetAddress<String>();
	return *left == *right;
}

void StringData::Serialize( DataInstance i, Stream& stream, ObjectIdentifier& identifier )
{
	const uint32_t length = static_cast< uint32_t >( i.GetAddress<String>()->GetSize() );

	DataHeader header;
	header.m_ValueType = ScalarDataTypes::String;
	header.m_Length = length + sizeof( length );
	header.Serialize( stream );

	stream.Write( length );
	stream.Write( i.GetAddress<String>()->GetData(), 1, length );
}

void StringData::Deserialize( DataInstance i, Stream& stream, ObjectResolver& resolver, bool raiseChanged )
{
	DataHeader header;
	header.Deserialize( stream );
	if ( header.m_ContainerType == ContainerDataTypes::Invalid && header.m_ValueType == ScalarDataTypes::String )
	{
		uint32_t length = 0;
		stream.Read( length );

		i.GetAddress<String>()->Resize( length );
		stream.Read( const_cast< char* >( i.GetAddress<String>()->GetData() ), 1, length );
		
		i.RaiseChanged( raiseChanged );
	}
	else
	{
		HELIUM_ASSERT( false );
		stream.Seek( header.m_Length - sizeof( header ), SeekOrigins::Current );
	}
}

void StringData::Serialize( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	string = *i.GetAddress<String>();
}

void StringData::Deserialize( DataInstance i, const String& string, ObjectResolver& resolver, bool raiseChanged )
{
	*i.GetAddress<String>() = string;
}

void StringData::Accept( DataInstance i, Visitor& visitor )
{
	visitor.VisitField( i.GetAddress< String >(), i.m_Field );
}
