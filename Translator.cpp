#include "ReflectPch.h"
#include "Reflect/Translator.h"

#include "Reflect/Structure.h"

using namespace Helium;
using namespace Helium::Reflect;

void DeferredResolver::Resolve( const Name& identity, ObjectPtr& pointer, const Class* pointerClass )
{
	Entry entry;
	entry.m_Pointer = &pointer;
	entry.m_PointerClass = pointerClass;
	entry.m_Identity = identity;
	m_Entries.Add( entry );
}
