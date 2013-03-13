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
void Helium::Reflect::ObjectResolver::Resolve( const Name& identity, StrongPtr< T >& object )
{
	const Class* pointerClass = Reflect::GetClass< T >();
	this->Resolve( identity, reinterpret_cast< ObjectPtr& >( object ), pointerClass );
}

template< class T >
T* Helium::Reflect::DataInstance::GetAddress(uintptr_t offsetInField) const
{
	HELIUM_ASSERT( m_Instance );
	HELIUM_ASSERT( m_Field );
	return reinterpret_cast< T* >( static_cast< char* >( m_Instance ) + m_Field->m_Offset + offsetInField );
}

Helium::Reflect::DataHeader::DataHeader()
	: m_Length( 0 )
	, m_ContainerType( 0 )
	, m_ValueType( 0 )
	, m_KeyType( 0 )
	, m_Pad( 0 )
{

}

bool Helium::Reflect::DataHeader::operator==( const DataHeader& rhs ) const
{
	return Helium::MemoryCompare( this, &rhs, sizeof( DataHeader ) ) == 0;
}

void Helium::Reflect::DataHeader::Serialize( Stream& stream ) const
{
	uint32_t length = m_Length;

#if HELIUM_ENDIAN_BIG
	Swizzle( length );
#endif

	stream.Write( length );
	stream.Write( m_ContainerType );
	stream.Write( m_ValueType );
	stream.Write( m_KeyType );
	stream.Write( m_Pad );
}

void Helium::Reflect::DataHeader::Deserialize( Stream& stream )
{
	stream.Read( m_Length );

#if HELIUM_ENDIAN_BIG
	Swizzle( m_Length );
#endif

	stream.Read( m_ContainerType );
	stream.Read( m_ValueType );
	stream.Read( m_KeyType );
	stream.Read( m_Pad );
}

template< class T >
uint32_t Helium::Reflect::DataHeader::GetLength( uint32_t extra ) const
{
	return sizeof( DataHeader ) + sizeof( T ) + extra;
}

template< class T >
uint32_t Helium::Reflect::DataHeader::SetLength( uint32_t extra )
{
	return m_Length = GetLength<T>( extra );
}