Helium::Reflect::DataInstance::DataInstance()
	: m_Instance( NULL )
	, m_Field( NULL )
{

}

Helium::Reflect::DataInstance::DataInstance(void* instance, const Field* field)
	: m_Instance( instance )
	, m_Field( field )
{

}

template< class T >
T* Helium::Reflect::DataInstance::GetAddress(uintptr_t offsetInField) const
{
	HELIUM_ASSERT( m_Instance );
	HELIUM_ASSERT( m_Field );
	return static_cast< T* >( static_cast< char* >( m_Instance ) + m_Field->m_Offset + offsetInField );
}

Helium::Reflect::DataHeader::DataHeader()
	: m_Length( 0 )
	, m_ContainerType( 0 )
	, m_ValueType( 0 )
	, m_KeyType( 0 )
	, m_Pad( 0 )
{

}

template< class T >
uint32_t Helium::Reflect::DataHeader::GetLength( uint32_t extra ) const
{
	return sizeof( DataHeader ) + sizeof( T ) + extra;
}

template< class T >
uint32_t Helium::Reflect::DataHeader::SetLength( uint32_t extra )
{
	m_Length = GetLength<T>( extra );
}