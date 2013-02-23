#include "ReflectPch.h"
#include "Reflect/ReflectionInfo.h"

using namespace Helium;
using namespace Helium::Reflect;

const tchar_t* ReflectionTypes::Strings[ ReflectionTypes::Count ] =
{
	TXT("Type"),
	TXT("Enumeration"),
	TXT("Composite"),
	TXT("Structure"),
	TXT("Class"),
	TXT("Data"),
	TXT("ContainerData"),
	TXT("SetData"),
	TXT("SequenceData"),
	TXT("AssociativeData")
};

ReflectionInfo::ReflectionInfo()
{

}

ReflectionInfo::~ReflectionInfo()
{

}