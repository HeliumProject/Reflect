#include "ReflectPch.h"
#include "Translators.h"

#include "Reflect/Object.h"

using namespace Helium;
using namespace Helium::Reflect;

void Reflect::Identify( ObjectIdentifier* identifier, Pointer pointer, Name& name )
{
	identifier->Identify( pointer.As< ObjectPtr >(), name );
}

void Reflect::Resolve( ObjectResolver* resolver, Name name, Pointer pointer )
{
	resolver->Resolve( name, pointer.As< ObjectPtr >() );
}

TypeTranslator::TypeTranslator()
	: ScalarTranslator( sizeof( const Type* ), ScalarTypes::String )
{
}

void TypeTranslator::Construct( Pointer pointer )
{
	DefaultConstruct< const Type* >( pointer );
}

void TypeTranslator::Destruct( Pointer pointer )
{
	DefaultDestruct< const Type* >( pointer );
}

void TypeTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< const Type* >( src, dest, flags );
}

bool TypeTranslator::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< const Type* >( a, b );
}

void TypeTranslator::Print( Pointer pointer, String& string, ObjectIdentifier* identifier)
{
	string.Clear();

	const Type* type = pointer.As< const Type* >();
	if ( type )
	{
		string = type->m_Name;
	}
}

void TypeTranslator::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	const Type* type = Registry::GetInstance()->GetType( string.GetData() );

	pointer.As< const Type* >() = type;

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}
