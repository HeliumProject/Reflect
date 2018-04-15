#include "Precompile.h"
#include "Reflect/Meta.h"

using namespace Helium;
using namespace Helium::Reflect;

const char* MetaIds::Strings[ MetaIds::Count ] =
{
	"MetaType",
	"MetaEnum",
	"MetaStruct",
	"MetaClass",
	"Translator",
	"ContainerTranslator",
	"SetTranslator",
	"SequenceTranslator",
	"AssociationTranslator"
};

HELIUM_COMPILE_ASSERT( sizeof( MetaIds::Strings ) / sizeof( MetaIds::Strings[0] ) == MetaIds::Count );

Meta::Meta()
{

}

Meta::~Meta()
{

}