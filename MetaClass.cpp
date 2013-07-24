#include "ReflectPch.h"
#include "Class.h"

#include "Reflect/Object.h"

using namespace Helium;
using namespace Helium::Reflect;

Class::Class()
: m_Creator (NULL)
{

}

Class::~Class()
{
	// prevent base class from deleting our object
	Structure::m_Default = NULL;
}

Class* Class::Create()
{
    return new Class();
}

template<>
void Class::Create< Object >( Class const*& pointer, const char* name, const char* baseName, CreateObjectFunc creator )
{
    Class* type = Class::Create();
    pointer = type;

    type->m_Size = sizeof( Object );
    type->m_Name = name;

    // object should have no creator
    HELIUM_ASSERT( creator == NULL );

    // object should have no base class
    HELIUM_ASSERT( baseName == NULL );
}
