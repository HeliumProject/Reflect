template< class T >
bool Helium::Reflect::SimpleData<T>::Copy( DataInstance src, DataInstance dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field );
	T* right = src.GetAddress<T>();
	T* left = dest.GetAddress<T>();
	*left = *right;
	return true;
}

template< class T >
bool Helium::Reflect::SimpleData<T>::Equals( DataInstance a, DataInstance b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field );
	T* right = a.GetAddress<T>();
	T* left = b.GetAddress<T>();
	return *left == *right;
}

template< class T >
void Helium::Reflect::SimpleData<T>::Accept( DataInstance i, Visitor& visitor )
{
	visitor.VisitField( i.GetAddress<T>(), i.m_Field );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Serialize( DataInstance i, Stream& stream, ObjectIdentifier& identifier )
{
	HELIUM_COMPILE_ASSERT( std::is_fundamental< T >::value );

	DataHeader header = GetDataHeader();
	header.Serialize( stream );

#if HELIUM_ENDIAN_BIG
	Swizzle( *i.GetAddress() );
#endif

	stream.Write( i.GetAddress<T>() );

#if HELIUM_ENDIAN_BIG
	Swizzle( *i.GetAddress() );
#endif
}

template< class T >
void Helium::Reflect::SimpleData<T>::Deserialize( DataInstance i, Stream& stream, ObjectResolver& resolver, bool raiseChanged )
{
	HELIUM_COMPILE_ASSERT( std::is_fundamental< T >::value );

	DataHeader header;
	header.Deserialize( stream );

	if ( GetDataHeader() == header )
	{
		stream.Read( *i.GetAddress<T>() );

#if HELIUM_ENDIAN_BIG
		Swizzle( *i.GetAddress<T>() );
#endif

		i.RaiseChanged( raiseChanged );
	}
	else
	{
		HELIUM_ASSERT( false );
		stream.Seek( header.m_Length - sizeof( header ), SeekOrigins::Current );
	}
}

template< class T >
void Helium::Reflect::SimpleData<T>::Serialize( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	HELIUM_COMPILE_ASSERT( std::is_fundamental< T >::value );

	std::stringstream str;
	str << *i.GetAddress<T>();
	string = str.str().c_str();
}

template< class T >
void Helium::Reflect::SimpleData<T>::Deserialize( DataInstance i, const String& string, ObjectResolver& resolver, bool raiseChanged )
{
	HELIUM_COMPILE_ASSERT( std::is_fundamental< T >::value );

	std::stringstream str ( string.GetData() );
	str >> *i.GetAddress<T>();
}

template< class T >
Helium::Reflect::ScalarDataType Helium::Reflect::SimpleData<T>::GetDataType()
{
	HELIUM_COMPILE_ASSERT( std::is_fundamental< T >::value );

	if ( std::is_integral< T >::value )
	{
		return ScalarDataTypes::Integer;
	}
	else if ( std::is_floating_point< T >::value )
	{
		return ScalarDataTypes::FloatingPoint;
	}

	HELIUM_ASSERT( false );
	return ScalarDataTypes::Invalid;
}

template< class T >
Helium::Reflect::DataHeader Helium::Reflect::SimpleData<T>::GetDataHeader()
{
	DataHeader header;
	header.m_ValueType = GetDataType();
	header.SetLength<T>();
	return header;
}

//
// Specializations
//

template<>
inline void Helium::Reflect::SimpleData<uint8_t>::Serialize( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	uint16_t v = *i.GetAddress<uint8_t>();

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
	
	*i.GetAddress<uint8_t>() = static_cast<uint8_t>( v );
}

template<>
inline void Helium::Reflect::SimpleData<int8_t>::Serialize( DataInstance i, String& string, ObjectIdentifier& identifier )
{
	int16_t v = *i.GetAddress<int8_t>();

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
	
	*i.GetAddress<int8_t>() = static_cast<int8_t>( v );
}
