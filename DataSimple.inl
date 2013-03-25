template< class T >
bool Helium::Reflect::SimpleData<T>::Copy( DataInstance src, DataInstance dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	T& right = src.As<T>();
	T& left = dest.As<T>();
	left = right;
	return true;
}

template< class T >
bool Helium::Reflect::SimpleData<T>::Equals( DataInstance a, DataInstance b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	T& right = a.As<T>();
	T& left = b.As<T>();
	return left == right;
}

template< class T >
void Helium::Reflect::SimpleData<T>::Accept( DataInstance i, Visitor& visitor )
{
	visitor.VisitField( this, i.m_Address, i.m_Field, i.m_Object );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Serialize( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	HELIUM_COMPILE_ASSERT( std::is_fundamental< T >::value );

	std::stringstream str;
	str << i.As<T>();
	string = str.str().c_str();
}

template< class T >
void Helium::Reflect::SimpleData<T>::Deserialize( DataInstance i, const String& string, ObjectResolver& resolver, bool raiseChanged )
{
	HELIUM_COMPILE_ASSERT( std::is_fundamental< T >::value );

	std::stringstream str ( string.GetData() );
	str >> i.As<T>();
}

//
// Specializations
//

template<>
inline void Helium::Reflect::SimpleData<uint8_t>::Serialize( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	uint16_t v = i.As<uint8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleData<uint8_t>::Deserialize( DataInstance i, const String& string, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	uint16_t v = 0;
	str >> v;
	
	i.As<uint8_t>() = static_cast<uint8_t>( v );
}

template<>
inline void Helium::Reflect::SimpleData<int8_t>::Serialize( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	int16_t v = i.As<int8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleData<int8_t>::Deserialize( DataInstance i, const String& string, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	int16_t v = 0;
	str >> v;
	
	i.As<int8_t>() = static_cast<int8_t>( v );
}
