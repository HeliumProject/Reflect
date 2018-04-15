#include "Precompile.h"
#include "Translator.h"

#include "Reflect/Object.h"
#include "Reflect/MetaStruct.h"

using namespace Helium;
using namespace Helium::Reflect;

Pointer::Pointer()
	: m_Address( 0x0 )
	, m_Field( 0 )
	, m_Object( 0 )
{

}

Pointer::Pointer( const Field* field, Object* object, uint32_t index )
	: m_Address( 0x0 )
	, m_Field( field )
	, m_Object( object )
{
	m_Address = reinterpret_cast< char* >( object ) + ( m_Field->m_Offset + ( ( m_Field->m_Size / m_Field->m_Count ) * index ) );
}

Pointer::Pointer( const Field* field, void* composite, Object* object, uint32_t index )
	: m_Address( 0x0 )
	, m_Field( field )
	, m_Object( object )
{
	m_Address = reinterpret_cast< char* >( composite ) + ( m_Field->m_Offset + ( ( m_Field->m_Size / m_Field->m_Count ) * index ) );
}

Pointer::Pointer( void *rawPtr, const Field* field, Object* object )
	: m_Address( rawPtr )
	, m_Field( field )
	, m_Object( object )
{

}

Pointer::Pointer( const Pointer& rhs )
	: m_Address( rhs.m_Address )
	, m_Field( rhs.m_Field )
	, m_Object( rhs.m_Object )
{

}

void Pointer::RaiseChanged( bool doIt )
{
	if ( doIt && m_Object )
	{
		m_Object->RaiseChanged( m_Field );
	}
}

uint32_t Translator::GetDefaultFlags()
{
	return 0x0;
}
