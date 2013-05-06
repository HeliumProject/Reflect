#include "ReflectPch.h"
#include "FoundationSupport.h"

using namespace Helium;
using namespace Helium::Reflect;

#pragma TODO("Use default template functions here -gorlak")

StringTranslator::StringTranslator()
	: ScalarTranslator( sizeof( String ), ScalarTypes::String )
{
}

void Helium::Reflect::StringTranslator::Construct( Pointer pointer )
{
	new ( pointer.m_Address ) String;
}

void Helium::Reflect::StringTranslator::Destruct( Pointer pointer )
{
	static_cast< String* >( pointer.m_Address )->~String();
}

void StringTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	String& right = src.As<String>();
	String& left = dest.As<String>();
	left = right;
}

bool StringTranslator::Equals( Pointer a, Pointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	String& right = a.As<String>();
	String& left = b.As<String>();
	return left == right;
}

void StringTranslator::Print( Pointer pointer, String& string, ObjectIdentifier& identifier )
{
	string = pointer.As<String>();
}

void StringTranslator::Parse( const String& string, Pointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	pointer.As<String>() = string;
}

void StringTranslator::Accept( Pointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}

NameTranslator::NameTranslator()
	: ScalarTranslator( sizeof( Name ), ScalarTypes::String )
{
}

void Helium::Reflect::NameTranslator::Construct( Pointer pointer )
{
	new ( pointer.m_Address ) String;
}

void Helium::Reflect::NameTranslator::Destruct( Pointer pointer )
{
	static_cast< Name* >( pointer.m_Address )->~Name();
}

void NameTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	Name& right = src.As<Name>();
	Name& left = dest.As<Name>();
	left = right;
}

bool NameTranslator::Equals( Pointer a, Pointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	Name& right = a.As<Name>();
	Name& left = b.As<Name>();
	return left == right;
}

void NameTranslator::Print( Pointer pointer, String& string, ObjectIdentifier& identifier )
{
	string = pointer.As<Name>().Get();
}

void NameTranslator::Parse( const String& string, Pointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	pointer.As<Name>().Set( string );
}

void NameTranslator::Accept( Pointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}

FilePathTranslator::FilePathTranslator()
	: ScalarTranslator( sizeof( FilePath ), ScalarTypes::String )
{
}

void Helium::Reflect::FilePathTranslator::Construct( Pointer pointer )
{
	new ( pointer.m_Address ) FilePath;
}

void Helium::Reflect::FilePathTranslator::Destruct( Pointer pointer )
{
	static_cast< FilePath* >( pointer.m_Address )->~FilePath();
}

void FilePathTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	FilePath& right = src.As<FilePath>();
	FilePath& left = dest.As<FilePath>();
	left = right;
}

bool FilePathTranslator::Equals( Pointer a, Pointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	FilePath& right = a.As<FilePath>();
	FilePath& left = b.As<FilePath>();
	return left == right;
}

void FilePathTranslator::Print( Pointer pointer, String& string, ObjectIdentifier& identifier )
{
	string = pointer.As<FilePath>();
}

void FilePathTranslator::Parse( const String& string, Pointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	pointer.As<FilePath>().Set( string );
}

void FilePathTranslator::Accept( Pointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}
