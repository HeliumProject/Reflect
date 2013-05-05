#include "ReflectPch.h"
#include "DataFoundation.h"

using namespace Helium;
using namespace Helium::Reflect;

StringData::StringData()
	: ScalarData( sizeof( String ), ScalarTypes::String )
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

NameData::NameData()
	: ScalarData( sizeof( Name ), ScalarTypes::String )
{
}

void Helium::Reflect::NameData::Construct( DataPointer pointer )
{
	new ( pointer.m_Address ) String;
}

void Helium::Reflect::NameData::Destruct( DataPointer pointer )
{
	static_cast< Name* >( pointer.m_Address )->~Name();
}

void NameData::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	Name& right = src.As<Name>();
	Name& left = dest.As<Name>();
	left = right;
}

bool NameData::Equals( DataPointer a, DataPointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	Name& right = a.As<Name>();
	Name& left = b.As<Name>();
	return left == right;
}

void NameData::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	string = pointer.As<Name>().Get();
}

void NameData::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	pointer.As<Name>().Set( string );
}

void NameData::Accept( DataPointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}

FilePathData::FilePathData()
	: ScalarData( sizeof( FilePath ), ScalarTypes::String )
{
}

void Helium::Reflect::FilePathData::Construct( DataPointer pointer )
{
	new ( pointer.m_Address ) FilePath;
}

void Helium::Reflect::FilePathData::Destruct( DataPointer pointer )
{
	static_cast< FilePath* >( pointer.m_Address )->~FilePath();
}

void FilePathData::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	FilePath& right = src.As<FilePath>();
	FilePath& left = dest.As<FilePath>();
	left = right;
}

bool FilePathData::Equals( DataPointer a, DataPointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	FilePath& right = a.As<FilePath>();
	FilePath& left = b.As<FilePath>();
	return left == right;
}

void FilePathData::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	string = pointer.As<FilePath>();
}

void FilePathData::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	pointer.As<FilePath>().Set( string );
}

void FilePathData::Accept( DataPointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}
