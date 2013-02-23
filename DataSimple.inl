template< class T >
bool Helium::Reflect::SimpleData<T>::Copy( DataInstance src, DataInstance dest, uint32_t flags )
{
	HELIUM_ASSERT( src.m_Field == dest.m_Field ); // NYI
	T* right = src.GetAddress<T>();
	T* left = dest.GetAddress<T>();
	*left = *right;
	return true;
}

template< class T >
bool Helium::Reflect::SimpleData<T>::Equals( DataInstance a, DataInstance b )
{
	HELIUM_ASSERT( a.m_Field == b.m_Field ); // NYI
	T* right = a.GetAddress<T>();
	T* left = b.GetAddress<T>();
	return *left == *right;
}

template< class T >
void Helium::Reflect::SimpleData<T>::Serialize( DataInstance i, Stream& stream )
{
	DataHeader header;
	header.m_ValueType = GetDataType();
	header.SetLength<T>();

#if HELIUM_ENDIAN_BIG
	header.Swizzle();
#endif
	stream.Write( &header, sizeof( header ), 1 );

#if HELIUM_ENDIAN_BIG
	Swizzle( *i.GetAddress() );
#endif
	stream.Write( i.GetAddress(), sizeof( T ), 1 );

#if HELIUM_ENDIAN_BIG
	Swizzle( *i.GetAddress() );
#endif
}

template< class T >
void Helium::Reflect::SimpleData<T>::Deserialize( DataInstance i, const Stream& stream, bool raiseChanged )
{
	DataHeader header;
	stream.Read( &header, sizeof( header ), 1 );
#if HELIUM_ENDIAN_BIG
	header.Swizzle();
#endif
	if ( header.m_ValueType == GetDataType() )
	{
		stream.Read( i.GetAddress(), sizeof( T ), 1 );
#if HELIUM_ENDIAN_BIG
		Swizzle( *i.GetAddress() );
#endif
		i.RaiseChanged( raiseChanged );
	}
	else
	{
		HELIUM_ASSERT( false );
		stream.Read( NULL, header.m_Length - sizeof( header ), 1 );
	}
}

template< class T >
void Helium::Reflect::SimpleData<T>::Serialize( DataInstance i, String& string )
{
	string.Print( *i.GetAddress() );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Deserialize( DataInstance i, const String& string, bool raiseChanged )
{
	string.Parse( i.GetAddress() );
	i.RaiseChanged( raiseChanged );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Accept( DataInstance i, Visitor& visitor )
{

}

template< class T >
Helium::Reflect::SimpleDataType Helium::Reflect::SimpleData<T>::GetDataType()
{
	HELIUM_ASSERT( false );
	return SimpleDataTypes::Invalid;
}


//
// Specializations
//

#if 0

template <>
void Helium::Reflect::SimpleData<tstring>::Serialize(ArchiveBinary& archive)
{
	archive.GetStream().WriteString( *m_Data ); 
}

template <>
void Helium::Reflect::SimpleData<tstring>::Deserialize(ArchiveBinary& archive)
{
	archive.GetStream().ReadString( *m_Data );
}

template <>
void Helium::Reflect::SimpleData<tstring>::Serialize(ArchiveXML& archive)
{
	archive.GetStream() << TXT( "<![CDATA[" ) << *m_Data << TXT( "]]>" );
}

template <>
void Helium::Reflect::SimpleData<tstring>::Deserialize(ArchiveXML& archive)
{
	std::streamsize size = archive.GetStream().ElementsAvailable(); 
	m_Data->resize( (size_t)size );
	archive.GetStream().ReadBuffer( const_cast<tchar_t*>( (*m_Data).c_str() ), size );
}

template<>
tostream& Helium::Reflect::SimpleData<tstring>::operator>>(tostream& stream) const
{
	stream << *m_Data;

	return stream;
}

template<>
tistream& Helium::Reflect::SimpleData<tstring>::operator<<(tistream& stream)
{
	std::streamsize size = stream.rdbuf()->in_avail();
	m_Data->resize( (size_t) size);
	stream.read( const_cast<tchar_t*>( (*m_Data).c_str()), size );

	return stream;
}

template <>
void Helium::Reflect::SimpleData<uint8_t>::Serialize(ArchiveBinary& archive)
{
	const uint8_t* data = m_Data;
	archive.GetStream().Write( data ); 
}

template <>
void Helium::Reflect::SimpleData<uint8_t>::Deserialize(ArchiveBinary& archive)
{
	const uint8_t* data = m_Data;
	archive.GetStream().Read( data ); 
}

template <>
void Helium::Reflect::SimpleData<uint8_t>::Serialize(ArchiveXML& archive)
{
	uint16_t tmp = *m_Data;
	archive.GetStream() << tmp;
}

template <>
void Helium::Reflect::SimpleData<uint8_t>::Deserialize(ArchiveXML& archive)
{
	uint16_t tmp;
	archive.GetStream() >> tmp;
	*m_Data = (uint8_t)tmp;
}

template<>
tostream& Helium::Reflect::SimpleData<uint8_t>::operator>>(tostream& stream) const
{
	uint16_t val = *m_Data;
	stream << val;
	return stream;
}

template<>
tistream& Helium::Reflect::SimpleData<uint8_t>::operator<<(tistream& stream)
{
	uint16_t val;
	stream >> val;
	*m_Data = (uint8_t)val;
	return stream;
}

template <>
void Helium::Reflect::SimpleData<int8_t>::Serialize(ArchiveBinary& archive)
{
	const int8_t* data = m_Data;
	archive.GetStream().Write( data ); 
}

template <>
void Helium::Reflect::SimpleData<int8_t>::Deserialize(ArchiveBinary& archive)
{
	const int8_t* data = m_Data;
	archive.GetStream().Read( data ); 
}

template <>
void Helium::Reflect::SimpleData<int8_t>::Serialize(ArchiveXML& archive)
{
	int16_t tmp = *m_Data;
	archive.GetStream() << tmp;
}

template <>
void Helium::Reflect::SimpleData<int8_t>::Deserialize(ArchiveXML& archive)
{
	int16_t tmp;
	archive.GetStream() >> tmp;
	*m_Data = (char)tmp;
}

template<>
tostream& Helium::Reflect::SimpleData<int8_t>::operator>>(tostream& stream) const
{
	int16_t val = *m_Data;
	stream << val;

	return stream;
}

template<>
tistream& Helium::Reflect::SimpleData<int8_t>::operator<<(tistream& stream)
{
	int16_t val;
	stream >> val;
	*m_Data = (uint8_t)val;

	return stream;
}

#endif