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
bool Helium::Reflect::EnumerationData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	return DefaultCopy<T>( src, dest, flags );
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
bool Helium::Reflect::PointerData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags )
{
	return DefaultCopy< StrongPtr< T > >( src, dest, flags );
}

template< class T >
bool Helium::Reflect::PointerData<T>::Equals( DataPointer a, DataPointer b )
{
	return DefaultEquals< StrongPtr< T > >( a, b );
}

template< class T >
void Helium::Reflect::PointerData<T>::Accept( DataPointer pointer, Visitor& visitor )
{
	DefaultAccept< StrongPtr< T > >( pointer, visitor );
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
