template<> Helium::Reflect::SimpleScalarData< uint8_t >::SimpleScalarData()   : ScalarData( 1, ScalarTypes::Unsigned8 ) {}
template<> Helium::Reflect::SimpleScalarData< uint16_t >::SimpleScalarData()  : ScalarData( 2, ScalarTypes::Unsigned16 ) {}
template<> Helium::Reflect::SimpleScalarData< uint32_t >::SimpleScalarData()  : ScalarData( 4, ScalarTypes::Unsigned32 ) {}
template<> Helium::Reflect::SimpleScalarData< uint64_t >::SimpleScalarData()  : ScalarData( 8, ScalarTypes::Unsigned64 ) {}
template<> Helium::Reflect::SimpleScalarData< int8_t >::SimpleScalarData()    : ScalarData( 1, ScalarTypes::Signed8 ) {}
template<> Helium::Reflect::SimpleScalarData< int16_t >::SimpleScalarData()   : ScalarData( 2, ScalarTypes::Signed16 ) {}
template<> Helium::Reflect::SimpleScalarData< int32_t >::SimpleScalarData()   : ScalarData( 4, ScalarTypes::Signed32 ) {}
template<> Helium::Reflect::SimpleScalarData< int64_t >::SimpleScalarData()   : ScalarData( 8, ScalarTypes::Signed64 ) {}
template<> Helium::Reflect::SimpleScalarData< float32_t >::SimpleScalarData() : ScalarData( 4, ScalarTypes::Float32 ) {}
template<> Helium::Reflect::SimpleScalarData< float64_t >::SimpleScalarData() : ScalarData( 8, ScalarTypes::Float64 ) {}

template< class T >
Helium::Reflect::SimpleScalarData<T>::SimpleScalarData()
{
	HELIUM_COMPILE_ASSERT( false );
}

template< class T >
void Helium::Reflect::SimpleScalarData<T>::Construct( DataPointer pointer )
{
	Data::DefaultConstruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleScalarData<T>::Destruct( DataPointer pointer )
{
	DefaultDestruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleScalarData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	DefaultCopy< T >( src, dest, flags );
}

template< class T >
bool Helium::Reflect::SimpleScalarData<T>::Equals( DataPointer a, DataPointer b )
{
	return DefaultEquals< T >( a, b );
}

template< class T >
void Helium::Reflect::SimpleScalarData<T>::Accept( DataPointer pointer, Visitor& visitor )
{
	DefaultAccept< T >( pointer, visitor );
}

template< class T >
void Helium::Reflect::SimpleScalarData<T>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	DefaultPrint< T >( pointer, string, identifier );
}

template< class T >
void Helium::Reflect::SimpleScalarData<T>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	DefaultParse< T >( string, pointer, resolver, raiseChanged );
}

template<>
inline void Helium::Reflect::SimpleScalarData<uint8_t>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	uint16_t v = pointer.As<uint8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleScalarData<uint8_t>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	uint16_t v = 0;
	str >> v;
	
	pointer.As<uint8_t>() = static_cast<uint8_t>( v );
}

template<>
inline void Helium::Reflect::SimpleScalarData<int8_t>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier )
{
	int16_t v = pointer.As<int8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleScalarData<int8_t>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	int16_t v = 0;
	str >> v;
	
	pointer.As<int8_t>() = static_cast<int8_t>( v );
}

//////////////////////////////////////////////////////////////////////////

template< class T >
Helium::Reflect::SimpleStructureData<T>::SimpleStructureData()
	: StructureData( sizeof( T ) )
{
}

template< class T >
void Helium::Reflect::SimpleStructureData<T>::Construct( DataPointer pointer )
{
	Data::DefaultConstruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleStructureData<T>::Destruct( DataPointer pointer )
{
	DefaultDestruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleStructureData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	const Structure* structure = Reflect::GetStructure< T >();
	structure->Copy( src.m_Address, src.m_Object, dest.m_Address, dest.m_Object );

	if ( flags & CopyFlags::Notify && dest.m_Object )
	{
		dest.m_Object->RaiseChanged( dest.m_Field ); 
	}
}

template< class T >
bool Helium::Reflect::SimpleStructureData<T>::Equals( DataPointer a, DataPointer b )
{
	const Structure* structure = Reflect::GetStructure< T >();
	return structure->Equals( a.m_Address, a.m_Object, b.m_Address, b.m_Object );

}

template< class T >
void Helium::Reflect::SimpleStructureData<T>::Accept( DataPointer pointer, Visitor& visitor )
{
	const Structure* structure = Reflect::GetStructure< T >();
	structure->Visit( pointer.m_Address, pointer.m_Object, visitor );
}

template< class T >
const Helium::Reflect::Structure* Helium::Reflect::SimpleStructureData<T>::GetStructure() const
{
	return Reflect::GetStructure< T >();
}

//////////////////////////////////////////////////////////////////////////

template< class T >
Helium::Reflect::PointerData<T>::PointerData()
	: ScalarData( sizeof( T ), ScalarTypes::String )
{
}

template< class T >
void Helium::Reflect::PointerData<T>::Construct( DataPointer pointer )
{
	DefaultConstruct< StrongPtr< T > >( pointer );
}

template< class T >
void Helium::Reflect::PointerData<T>::Destruct( DataPointer pointer )
{
	DefaultDestruct< StrongPtr< T > >( pointer );
}

template< class T >
void Helium::Reflect::PointerData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	if ( flags & CopyFlags::Shallow )
	{
		DefaultCopy< StrongPtr< T > >( src, dest, flags );
	}
	else
	{
		src.As< StrongPtr< T > >()->CopyTo( dest.As< StrongPtr< T > >() );
	}

	if ( flags & CopyFlags::Notify && dest.m_Object )
	{
		dest.m_Object->RaiseChanged( dest.m_Field ); 
	}
}

template< class T >
bool Helium::Reflect::PointerData<T>::Equals( DataPointer a, DataPointer b )
{
	if ( DefaultEquals< StrongPtr< T > >( a, b ) )
	{
		return true;
	}
	
	return a.As< StrongPtr< T > >()->Equals( b.As< StrongPtr< T > >() );
}

template< class T >
void Helium::Reflect::PointerData<T>::Accept( DataPointer pointer, Visitor& visitor )
{
	if ( !visitor.VisitPointer( pointer.As< ObjectPtr >() ) )
	{
		return;
	}

	pointer.As< StrongPtr< T > >()->Accept( visitor );
}

template< class T >
void Helium::Reflect::PointerData<T>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier)
{
	Name name;
	identifier.Identify( pointer.As< StrongPtr< T > >(), name );
	string = name.Get();
}

template< class T >
void Helium::Reflect::PointerData<T>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	resolver.Resolve( Name( string ), pointer.As< StrongPtr< T > >() );

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}

//////////////////////////////////////////////////////////////////////////

template< class T >
Helium::Reflect::EnumerationData<T>::EnumerationData()
	: ScalarData( sizeof( T ), ScalarTypes::String )
{
}

template< class T >
void Helium::Reflect::EnumerationData<T>::Construct( DataPointer pointer )
{
	DefaultConstruct<T>( pointer );
}

template< class T >
void Helium::Reflect::EnumerationData<T>::Destruct( DataPointer pointer )
{
	DefaultDestruct<T>( pointer );
}

template< class T >
void Helium::Reflect::EnumerationData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	DefaultCopy<T>( src, dest, flags );
}

template< class T >
bool Helium::Reflect::EnumerationData<T>::Equals( DataPointer a, DataPointer b )
{
	return DefaultEquals<T>( a, b );
}

template< class T >
void Helium::Reflect::EnumerationData<T>::Accept( DataPointer pointer, Visitor& visitor )
{
	DefaultAccept<T>( pointer, visitor );
}

template< class T >
void Helium::Reflect::EnumerationData<T>::Print( DataPointer pointer, String& string, ObjectIdentifier& identifier)
{
	const Enumeration* enumeration = GetEnumeration< T >();
	tstring str;
	enumeration->GetString( pointer.As< T >(), str );
	string = str.c_str();
}

template< class T >
void Helium::Reflect::EnumerationData<T>::Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged )
{
	const Enumeration* enumeration = GetEnumeration< T >();
	tstring str = string;
	enumeration->GetValue( str, pointer.As< T >() );

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}
