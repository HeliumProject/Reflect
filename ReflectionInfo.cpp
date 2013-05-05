#include "ReflectPch.h"
#include "Reflect/ReflectionInfo.h"

using namespace Helium;
using namespace Helium::Reflect;

const tchar_t* ReflectionTypes::Strings[ ReflectionTypes::Count ] =
{
	TXT("Type"),
	TXT("Enumeration"),
	TXT("Structure"),
	TXT("Class"),
	TXT("Translator"),
	TXT("ContainerTranslator"),
	TXT("SetTranslator"),
	TXT("SequenceTranslator"),
	TXT("AssociationTranslator")
};

HELIUM_COMPILE_ASSERT( sizeof( ReflectionTypes::Strings ) / sizeof( ReflectionTypes::Strings[0] ) == ReflectionTypes::Count );

ReflectionInfo::ReflectionInfo()
{

}

ReflectionInfo::~ReflectionInfo()
{

}