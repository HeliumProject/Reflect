#include "ReflectPch.h"
#include "DataFoundation.h"

using namespace Helium;
using namespace Helium::Reflect;

bool StringData::Copy( DataInstance src, DataInstance dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	String& right = src.As<String>();
	String& left = dest.As<String>();
	left = right;
	return true;
}

bool StringData::Equals( DataInstance a, DataInstance b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	String& right = a.As<String>();
	String& left = b.As<String>();
	return left == right;
}

void StringData::Print( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	string = i.As<String>();
}

void StringData::Parse( const String& string, DataInstance i, ObjectResolver& resolver, bool raiseChanged )
{
	i.As<String>() = string;
}

void StringData::Accept( DataInstance i, Visitor& visitor )
{
	visitor.VisitField( this, i.m_Address, i.m_Field, i.m_Object );
}
