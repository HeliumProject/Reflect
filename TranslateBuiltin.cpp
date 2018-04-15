#include "Precompile.h"
#include "TranslateBuiltin.h"

#include "Reflect/Object.h"

using namespace Helium;
using namespace Helium::Reflect;

bool Reflect::Identify( ObjectIdentifier* identifier, Pointer pointer, Name* name )
{
	return identifier->Identify( pointer.As< ObjectPtr >(), name );
}

void Reflect::Resolve( ObjectResolver* resolver, Name name, Pointer pointer )
{
	resolver->Resolve( name, pointer.As< ObjectPtr >() );
}

TypeTranslator::TypeTranslator()
	: ScalarTranslator( sizeof( const MetaType* ), ScalarTypes::String )
{
}

void TypeTranslator::Construct( Pointer pointer )
{
	DefaultConstruct< const MetaType* >( pointer );
}

void TypeTranslator::Destruct( Pointer pointer )
{
	DefaultDestruct< const MetaType* >( pointer );
}

void TypeTranslator::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< const MetaType* >( src, dest, flags );
}

bool TypeTranslator::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< const MetaType* >( a, b );
}

void TypeTranslator::Print( Pointer pointer, String& string, ObjectIdentifier* identifier)
{
	string.Clear();

	const MetaType* type = pointer.As< const MetaType* >();
	if ( type )
	{
		string = type->m_Name;
	}
}

void TypeTranslator::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	const MetaType* type = Registry::GetInstance()->GetType( string.GetData() );

	pointer.As< const MetaType* >() = type;

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}
