#include "Precompile.h"
#include "TranslateStandardLibrary.h"

#include "Reflect/Object.h"

using namespace Helium;
using namespace Helium::Reflect;

StlStringTranslator::StlStringTranslator()
	: ScalarTranslator( sizeof( std::string ), ScalarTypes::String )
{
}

void Helium::Reflect::StlStringTranslator::Construct( Pointer pointer )
{
	DefaultConstruct< std::string >( pointer );
}

void Helium::Reflect::StlStringTranslator::Destruct( Pointer pointer )
{
	DefaultDestruct< std::string >( pointer );
}

void StlStringTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< std::string >( src, dest, flags );
}

bool StlStringTranslator::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< std::string >( a, b );
}

void StlStringTranslator::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	string = pointer.As< std::string >().c_str();
}

void StlStringTranslator::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	pointer.As< std::string >() = string.GetData();
}
