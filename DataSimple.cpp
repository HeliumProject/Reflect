#include "ReflectPch.h"
#include "DataSimple.h"

using namespace Helium;
using namespace Helium::Reflect;

TypeData::TypeData()
	: ScalarData( sizeof( const Type* ), ScalarTypes::String )
{
}

void TypeData::Construct( DataPointer pointer )
{
	DefaultConstruct< const Type* >( pointer );
}

void TypeData::Destruct( DataPointer pointer )
{
	DefaultDestruct< const Type* >( pointer );
}

void TypeData::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	DefaultCopy< const Type* >( src, dest, flags );
}

bool TypeData::Equals( DataPointer a, DataPointer b )
{
	return DefaultEquals< const Type* >( a, b );
}

void TypeData::Accept( DataPointer pointer, Visitor& visitor )
{
	DefaultAccept< const Type* >( pointer, visitor );
}

void TypeData::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier)
{
	string.Clear();

	const Type* type = pointer.As< const Type* >();
	if ( type )
	{
		string = type->m_Name;
	}
}

void TypeData::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	const Type* type = Registry::GetInstance()->GetType( string.GetData() );

	pointer.As< const Type* >() = type;

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}
