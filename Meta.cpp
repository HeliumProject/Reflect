#include "ReflectPch.h"
#include "Reflect/Meta.h"

using namespace Helium;
using namespace Helium::Reflect;

const char* MetaIds::Strings[ MetaIds::Count ] =
{
	TXT("MetaType"),
	TXT("MetaEnum"),
	TXT("MetaStruct"),
	TXT("MetaClass"),
	TXT("Translator"),
	TXT("ContainerTranslator"),
	TXT("SetTranslator"),
	TXT("SequenceTranslator"),
	TXT("AssociationTranslator")
};

HELIUM_COMPILE_ASSERT( sizeof( MetaIds::Strings ) / sizeof( MetaIds::Strings[0] ) == MetaIds::Count );

Meta::Meta()
{

}

Meta::~Meta()
{

}