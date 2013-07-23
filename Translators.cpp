#include "ReflectPch.h"
#include "Translators.h"

#include "Reflect/Object.h"

using namespace Helium;
using namespace Helium::Reflect;

template<> Helium::Reflect::SimpleScalarTranslator< bool >::SimpleScalarTranslator()      : ScalarTranslator( 1, ScalarTypes::Boolean ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint8_t >::SimpleScalarTranslator()   : ScalarTranslator( 1, ScalarTypes::Unsigned8 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint16_t >::SimpleScalarTranslator()  : ScalarTranslator( 2, ScalarTypes::Unsigned16 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint32_t >::SimpleScalarTranslator()  : ScalarTranslator( 4, ScalarTypes::Unsigned32 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint64_t >::SimpleScalarTranslator()  : ScalarTranslator( 8, ScalarTypes::Unsigned64 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int8_t >::SimpleScalarTranslator()    : ScalarTranslator( 1, ScalarTypes::Signed8 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int16_t >::SimpleScalarTranslator()   : ScalarTranslator( 2, ScalarTypes::Signed16 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int32_t >::SimpleScalarTranslator()   : ScalarTranslator( 4, ScalarTypes::Signed32 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int64_t >::SimpleScalarTranslator()   : ScalarTranslator( 8, ScalarTypes::Signed64 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< float32_t >::SimpleScalarTranslator() : ScalarTranslator( 4, ScalarTypes::Float32 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< float64_t >::SimpleScalarTranslator() : ScalarTranslator( 8, ScalarTypes::Float64 ) {}

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
