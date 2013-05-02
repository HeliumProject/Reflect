Helium::Reflect::DataPointer::DataPointer()
	: m_Address( 0x0 )
	, m_Field( 0 )
	, m_Object( 0 )
{

}

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

Helium::Reflect::DataPointer::DataPointer( void *rawPtr, const Field* field, Object* object )
	: m_Address( rawPtr )
	, m_Field( field )
	, m_Object( object )
{

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
	: DataPointer( static_cast< Field* >( NULL ), NULL, static_cast< Object* >( NULL ) )
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

template< class T >
void Helium::Reflect::Data::DefaultConstruct( DataPointer pointer )
{
	new ( pointer.m_Address ) T;
}

template< class T >
void Helium::Reflect::Data::DefaultDestruct( DataPointer pointer )
{
	static_cast< T* >( pointer.m_Address )->~T();
}

template< class T >
void Helium::Reflect::Data::DefaultCopy( DataPointer src, DataPointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	T& right = src.As<T>();
	T& left = dest.As<T>();
	left = right;
}

template< class T >
bool Helium::Reflect::Data::DefaultEquals( DataPointer a, DataPointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	T& right = a.As<T>();
	T& left = b.As<T>();
	return left == right;
}

template< class T >
void Helium::Reflect::Data::DefaultAccept( DataPointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}

Helium::Reflect::Data::Data( size_t size )
	: m_Size( size )
{
}

Helium::Reflect::ScalarData::ScalarData( size_t size, ScalarType type )
	: Data( size )
	, m_Type( type )
{
}

template< class T >
void Helium::Reflect::ScalarData::DefaultPrint( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	std::stringstream str;
	str << pointer.As<T>();
	string = str.str().c_str();
}

template< class T >
void Helium::Reflect::ScalarData::DefaultParse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	str >> pointer.As<T>();

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}

Helium::Reflect::StructureData::StructureData( size_t size )
	: Data( size )
{

}

Helium::Reflect::ContainerData::ContainerData( size_t size )
    : Data(size)
{

}

Helium::Reflect::SetData::SetData( size_t size )
    : ContainerData(size)
{

}

Helium::Reflect::SequenceData::SequenceData( size_t size )
    : ContainerData(size)
{

}

Helium::Reflect::AssociationData::AssociationData( size_t size )
    : ContainerData(size)
{

}