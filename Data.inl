Helium::Reflect::DataInstance::DataInstance( const Field* field, Object* object )
	: m_Object( object )
	, m_Field( field )
	, m_Address( 0x0 )
{
	m_Address = reinterpret_cast< char* >( object ) + m_Field->m_Offset;
}

Helium::Reflect::DataInstance::DataInstance( Structure* baseAddress, const Field* field, Object* object )
	: m_Object( object )
	, m_Field( field )
	, m_Address( 0x0 )
{
	m_Address = reinterpret_cast< char* >( baseAddress ) + m_Field->m_Offset;
}

Helium::Reflect::DataInstance::DataInstance( void* finalAddress, const Field* field, Object* object )
	: m_Object( object )
	, m_Field( field )
	, m_Address( finalAddress )
{

}

Helium::Reflect::DataInstance::DataInstance( const DataInstance& rhs )
	: m_Object( rhs.m_Object )
	, m_Field( rhs.m_Field )
	, m_Address( rhs.m_Address )
{

}

template< class T >
T& Helium::Reflect::DataInstance::As() const
{
	return *reinterpret_cast< T* >( m_Address );
}

template< class T >
void Helium::Reflect::ObjectResolver::Resolve( const Name& identity, StrongPtr< T >& object )
{
	const Class* pointerClass = Reflect::GetClass< T >();
	this->Resolve( identity, reinterpret_cast< ObjectPtr& >( object ), pointerClass );
}

Helium::Reflect::DeferredResolver::Entry::Entry()
	: m_Pointer( NULL )
	, m_PointerClass( NULL )
{

}

Helium::Reflect::ScalarData::ScalarData( ScalarType t )
	: m_Type( t )
{
}