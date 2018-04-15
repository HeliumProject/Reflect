#include "Precompile.h"
#include "MetaClass.h"

#include "Reflect/Object.h"

using namespace Helium;
using namespace Helium::Reflect;

MetaClass::MetaClass()
: m_Creator (NULL)
{

}

MetaClass::~MetaClass()
{
	if ( MetaClass::m_Default )
	{
		// an referencers of the default instance should use weak references!
		HELIUM_ASSERT( ( MetaClass::m_Default.GetProxy()->GetRefCounts() & 0xffff ) == 1 );

		// prevent base class from (double) deleting our default instance
		MetaStruct::m_Default = NULL;
	}
}

MetaClass* MetaClass::Create()
{
    return new MetaClass();
}

template<>
void MetaClass::Create< Object >( MetaClass const*& pointer, const char* name, const char* baseName, CreateObjectFunc creator )
{
    MetaClass* type = MetaClass::Create();
    pointer = type;

    type->m_Size = sizeof( Object );
    type->m_Name = name;

    // object should have no creator
    HELIUM_ASSERT( creator == NULL );

    // object should have no base class
    HELIUM_ASSERT( baseName == NULL );
}
