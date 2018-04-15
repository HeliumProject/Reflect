#include "Precompile.h"
#include "TranslateFoundation.h"

#include "Reflect/Object.h"

using namespace Helium;
using namespace Helium::Reflect;

StringTranslator::StringTranslator()
	: ScalarTranslator( sizeof( String ), ScalarTypes::String )
{
}

void Helium::Reflect::StringTranslator::Construct( Pointer pointer )
{
	DefaultConstruct< String >( pointer );
}

void Helium::Reflect::StringTranslator::Destruct( Pointer pointer )
{
	DefaultDestruct< String >( pointer );
}

void StringTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< String >( src, dest, flags );
}

bool StringTranslator::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< String >( a, b );
}

void StringTranslator::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	string = pointer.As<String>();
}

void StringTranslator::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	pointer.As<String>() = string;
}

NameTranslator::NameTranslator()
	: ScalarTranslator( sizeof( Name ), ScalarTypes::String )
{
}

void Helium::Reflect::NameTranslator::Construct( Pointer pointer )
{
	DefaultConstruct< Name >( pointer );
}

void Helium::Reflect::NameTranslator::Destruct( Pointer pointer )
{
	DefaultDestruct< Name >( pointer );
}

void NameTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< Name >( src, dest, flags );
}

bool NameTranslator::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< Name >( a, b );
}

void NameTranslator::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	string = pointer.As<Name>().Get();
}

void NameTranslator::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	pointer.As<Name>().Set( string );
}

FilePathTranslator::FilePathTranslator()
	: ScalarTranslator( sizeof( FilePath ), ScalarTypes::String )
{
}

void Helium::Reflect::FilePathTranslator::Construct( Pointer pointer )
{
	DefaultConstruct< FilePath >( pointer );
}

void Helium::Reflect::FilePathTranslator::Destruct( Pointer pointer )
{
	DefaultDestruct< FilePath >( pointer.m_Address );
}

void FilePathTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< FilePath >( src, dest, flags );
}

bool FilePathTranslator::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< FilePath >( a, b );
}

void FilePathTranslator::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	string = pointer.As<FilePath>().Data();
}

void FilePathTranslator::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	pointer.As<FilePath>().Set( string );
}
