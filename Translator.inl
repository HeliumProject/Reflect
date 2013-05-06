Helium::Reflect::Pointer::Pointer()
	: m_Address( 0x0 )
	, m_Field( 0 )
	, m_Object( 0 )
{

}

Helium::Reflect::Pointer::Pointer( const Field* field, Object* object, uint32_t index )
	: m_Address( 0x0 )
	, m_Field( field )
	, m_Object( object )
{
	m_Address = reinterpret_cast< char* >( object ) + ( m_Field->m_Offset + ( ( m_Field->m_Size / m_Field->m_Count ) * index ) );
}

Helium::Reflect::Pointer::Pointer( const Field* field, void* composite, Object* object, uint32_t index )
	: m_Address( 0x0 )
	, m_Field( field )
	, m_Object( object )
{
	m_Address = reinterpret_cast< char* >( composite ) + ( m_Field->m_Offset + ( ( m_Field->m_Size / m_Field->m_Count ) * index ) );
}

Helium::Reflect::Pointer::Pointer( void *rawPtr, const Field* field, Object* object )
	: m_Address( rawPtr )
	, m_Field( field )
	, m_Object( object )
{

}

Helium::Reflect::Pointer::Pointer( const Pointer& rhs )
	: m_Address( rhs.m_Address )
	, m_Field( rhs.m_Field )
	, m_Object( rhs.m_Object )
{

}

template< class T >
T& Helium::Reflect::Pointer::As()
{
	return *reinterpret_cast< T* >( m_Address );
}

void Helium::Reflect::Pointer::RaiseChanged( bool doIt )
{
	if ( doIt && m_Object )
	{
		m_Object->RaiseChanged( m_Field );
	}
}

Helium::Reflect::Variable::Variable( Translator* translator )
	: Pointer( static_cast< Field* >( NULL ), NULL, static_cast< Object* >( NULL ) )
	, m_Translator( translator )
{
	m_Address = new unsigned char[ m_Translator->m_Size ];
	m_Translator->Construct( *this );
}

Helium::Reflect::Variable::~Variable()
{
	m_Translator->Destruct( *this );
	delete[] m_Address;
}

template< class T >
bool Helium::Reflect::ObjectResolver::Resolve( const Name& identity, StrongPtr< T >& object )
{
	const Class* pointerClass = Reflect::GetClass< T >();
	return this->Resolve( identity, reinterpret_cast< ObjectPtr& >( object ), pointerClass );
}

Helium::Reflect::DeferredResolver::Entry::Entry()
	: m_Pointer( NULL )
	, m_PointerClass( NULL )
{

}

template< class T >
void Helium::Reflect::Translator::DefaultConstruct( Pointer pointer )
{
	new ( pointer.m_Address ) T;
}

template< class T >
void Helium::Reflect::Translator::DefaultDestruct( Pointer pointer )
{
	static_cast< T* >( pointer.m_Address )->~T();
}

template< class T >
void Helium::Reflect::Translator::DefaultCopy( Pointer src, Pointer dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	T& right = src.As<T>();
	T& left = dest.As<T>();
	left = right;
}

template< class T >
bool Helium::Reflect::Translator::DefaultEquals( Pointer a, Pointer b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	T& right = a.As<T>();
	T& left = b.As<T>();
	return left == right;
}

template< class T >
void Helium::Reflect::Translator::DefaultAccept( Pointer pointer, Visitor& visitor )
{
	visitor.VisitField( this, pointer.m_Address, pointer.m_Field, pointer.m_Object );
}

Helium::Reflect::Translator::Translator( size_t size )
	: m_Size( size )
{
}

Helium::Reflect::ScalarTranslator::ScalarTranslator( size_t size, ScalarType type )
	: Translator( size )
	, m_Type( type )
{
}

template< class T >
void Helium::Reflect::ScalarTranslator::DefaultPrint( Pointer pointer, String& string, ObjectIdentifier& identifier )
{
	std::stringstream str;
	str << pointer.As<T>();
	string = str.str().c_str();
}

template< class T >
void Helium::Reflect::ScalarTranslator::DefaultParse( const String& string, Pointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	str >> pointer.As<T>();

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}

Helium::Reflect::StructureTranslator::StructureTranslator( size_t size )
	: Translator( size )
{

}

Helium::Reflect::ContainerTranslator::ContainerTranslator( size_t size )
	: Translator(size)
{

}

Helium::Reflect::SetTranslator::SetTranslator( size_t size )
	: ContainerTranslator(size)
{

}

Helium::Reflect::SequenceTranslator::SequenceTranslator( size_t size )
	: ContainerTranslator(size)
{

}

Helium::Reflect::AssociationTranslator::AssociationTranslator( size_t size )
	: ContainerTranslator(size)
{

}