Helium::Reflect::DataPointer::DataPointer( const Field* field, Object* object, uint32_t index )
	: m_Address( 0x0 )
	, m_Field( field )
	, m_Object( object )
{
	m_Address = reinterpret_cast< char* >( object ) + ( m_Field->m_Offset + ( ( m_Field->m_Size / m_Field->m_Count ) * index ) );
}

Helium::Reflect::DataPointer::DataPointer( const Field* field, void* composite, Object* object, uint32_t index )
	: m_Address( 0x0 )
	, m_Field( field )
	, m_Object( object )
{
	m_Address = reinterpret_cast< char* >( composite ) + ( m_Field->m_Offset + ( ( m_Field->m_Size / m_Field->m_Count ) * index ) );
}

Helium::Reflect::DataPointer::DataPointer( const DataPointer& rhs )
	: m_Address( rhs.m_Address )
	, m_Field( rhs.m_Field )
	, m_Object( rhs.m_Object )
{

}

template< class T >
T& Helium::Reflect::DataPointer::As()
{
	return *reinterpret_cast< T* >( m_Address );
}

void Helium::Reflect::DataPointer::RaiseChanged( bool doIt )
{
	if ( doIt && m_Object )
	{
		m_Object->RaiseChanged( m_Field );
	}
}

Helium::Reflect::DataVariable::DataVariable( Data* data )
	: DataPointer( NULL, NULL, static_cast< Object* >( NULL ) )
	, m_Data( data )
{
	m_Address = new unsigned char[ m_Data->m_Size ];
	m_Data->Construct( *this );
}

Helium::Reflect::DataVariable::~DataVariable()
{
	m_Data->Destruct( *this );
	delete[] m_Address;
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

Helium::Reflect::Data::Data( size_t size )
	: m_Size( size )
{
}

Helium::Reflect::ScalarData::ScalarData( size_t size, ScalarType t )
	: Data( size )
	, m_Type( t )
{
}