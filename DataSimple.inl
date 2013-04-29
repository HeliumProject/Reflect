template<> Helium::Reflect::SimpleData< uint8_t >::SimpleData()   : ScalarData( 1, ScalarTypes::Unsigned8 ) {}
template<> Helium::Reflect::SimpleData< uint16_t >::SimpleData()  : ScalarData( 2, ScalarTypes::Unsigned16 ) {}
template<> Helium::Reflect::SimpleData< uint32_t >::SimpleData()  : ScalarData( 4, ScalarTypes::Unsigned32 ) {}
template<> Helium::Reflect::SimpleData< uint64_t >::SimpleData()  : ScalarData( 8, ScalarTypes::Unsigned64 ) {}
template<> Helium::Reflect::SimpleData< int8_t >::SimpleData()    : ScalarData( 1, ScalarTypes::Signed8 ) {}
template<> Helium::Reflect::SimpleData< int16_t >::SimpleData()   : ScalarData( 2, ScalarTypes::Signed16 ) {}
template<> Helium::Reflect::SimpleData< int32_t >::SimpleData()   : ScalarData( 4, ScalarTypes::Signed32 ) {}
template<> Helium::Reflect::SimpleData< int64_t >::SimpleData()   : ScalarData( 8, ScalarTypes::Signed64 ) {}
template<> Helium::Reflect::SimpleData< float32_t >::SimpleData() : ScalarData( 4, ScalarTypes::Float32 ) {}
template<> Helium::Reflect::SimpleData< float64_t >::SimpleData() : ScalarData( 8, ScalarTypes::Float64 ) {}

template< class T >
Helium::Reflect::SimpleData<T>::SimpleData()
{
	HELIUM_COMPILE_ASSERT( false );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Construct( DataPointer pointer )
{
	Data::DefaultConstruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Destruct( DataPointer pointer )
{
	DefaultDestruct< T >( pointer );
}

template< class T >
bool Helium::Reflect::SimpleData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	return DefaultCopy< T >( src, dest, flags );
}

template< class T >
bool Helium::Reflect::SimpleData<T>::Equals( DataPointer a, DataPointer b )
{
	return DefaultEquals< T >( a, b );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Accept( DataPointer pointer, Visitor& visitor )
{
	DefaultAccept< T >( pointer, visitor );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	DefaultPrint< T >( pointer, string, identifier );
}

template< class T >
void Helium::Reflect::SimpleData<T>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	DefaultParse< T >( string, pointer, resolver, raiseChanged );
}

//
// Specializations
//

template<>
inline void Helium::Reflect::SimpleData<uint8_t>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	uint16_t v = pointer.As<uint8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleData<uint8_t>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	uint16_t v = 0;
	str >> v;
	
	pointer.As<uint8_t>() = static_cast<uint8_t>( v );
}

template<>
inline void Helium::Reflect::SimpleData<int8_t>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	int16_t v = pointer.As<int8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleData<int8_t>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	int16_t v = 0;
	str >> v;
	
	pointer.As<int8_t>() = static_cast<int8_t>( v );
}
