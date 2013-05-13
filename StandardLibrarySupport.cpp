#include "ReflectPch.h"
#include "StandardLibrarySupport.h"

using namespace Helium;
using namespace Helium::Reflect;

StlStringTranslator::StlStringTranslator()
	: ScalarTranslator( sizeof( tstring ), ScalarTypes::String )
{
}

void Helium::Reflect::StlStringTranslator::Construct( Pointer pointer )
{
	DefaultConstruct< tstring >( pointer );
}

void Helium::Reflect::StlStringTranslator::Destruct( Pointer pointer )
{
	DefaultDestruct< tstring >( pointer );
}

void StlStringTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< tstring >( src, dest, flags );
}

bool StlStringTranslator::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< tstring >( a, b );
}

void StlStringTranslator::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	string = pointer.As< tstring >().c_str();
}

void StlStringTranslator::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	pointer.As< tstring >() = string.GetData();
}

void StlStringTranslator::Accept( Pointer pointer, Visitor& visitor )
{
	DefaultAccept< tstring >( pointer, visitor );
}