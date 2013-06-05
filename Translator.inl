template< class T >
T& Helium::Reflect::Pointer::As()
{
	return *reinterpret_cast< T* >( m_Address );
}

Helium::Reflect::Variable::Variable( Translator* translator )
	: m_Translator( translator )
{
	m_Address = new unsigned char[ m_Translator->m_Size ];
	m_Translator->Construct( *this );
}

Helium::Reflect::Variable::~Variable()
{
	m_Translator->Destruct( *this );
	delete[] static_cast< char* >( m_Address );
}

Helium::Reflect::Data::Data( Pointer pointer, Translator* translator )
	: m_Pointer( pointer )
	, m_Translator( translator )
{
}

Helium::Reflect::Data::Data( const Data& rhs )
	: m_Pointer( rhs.m_Pointer )
	, m_Translator( rhs.m_Translator )
{
}

bool Helium::Reflect::Data::operator==( const Data& rhs )
{
	HELIUM_ASSERT( m_Translator == rhs.m_Translator );
	if ( m_Translator == rhs.m_Translator )
	{
		return m_Translator->Equals( m_Pointer, rhs.m_Pointer );
	}
	return false;
}

bool Helium::Reflect::Data::operator!=( const Data& rhs )
{
	return !this->operator==( rhs );
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
	HELIUM_ASSERT( !src.m_Field || !dest.m_Field || src.m_Field == dest.m_Field );
	T& right = src.As<T>();
	T& left = dest.As<T>();
	left = right;
}

template< class T >
bool Helium::Reflect::Translator::DefaultEquals( Pointer a, Pointer b )
{
	HELIUM_ASSERT( !a.m_Field || !b.m_Field || a.m_Field == b.m_Field );
	T& right = a.As<T>();
	T& left = b.As<T>();
	return left == right;
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
void Helium::Reflect::ScalarTranslator::DefaultPrint( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	std::stringstream str;
	str << pointer.As<T>();
	string = str.str().c_str();
}

template< class T >
void Helium::Reflect::ScalarTranslator::DefaultParse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	str >> pointer.As<T>();
	pointer.RaiseChanged( raiseChanged ); 
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