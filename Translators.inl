template<> Helium::Reflect::SimpleScalarTranslator< bool >::SimpleScalarTranslator()      : ScalarTranslator( 1, ScalarTypes::Boolean ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint8_t >::SimpleScalarTranslator()   : ScalarTranslator( 1, ScalarTypes::Unsigned8 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint16_t >::SimpleScalarTranslator()  : ScalarTranslator( 2, ScalarTypes::Unsigned16 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint32_t >::SimpleScalarTranslator()  : ScalarTranslator( 4, ScalarTypes::Unsigned32 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< uint64_t >::SimpleScalarTranslator()  : ScalarTranslator( 8, ScalarTypes::Unsigned64 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int8_t >::SimpleScalarTranslator()    : ScalarTranslator( 1, ScalarTypes::Signed8 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int16_t >::SimpleScalarTranslator()   : ScalarTranslator( 2, ScalarTypes::Signed16 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int32_t >::SimpleScalarTranslator()   : ScalarTranslator( 4, ScalarTypes::Signed32 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< int64_t >::SimpleScalarTranslator()   : ScalarTranslator( 8, ScalarTypes::Signed64 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< float32_t >::SimpleScalarTranslator() : ScalarTranslator( 4, ScalarTypes::Float32 ) {}
template<> Helium::Reflect::SimpleScalarTranslator< float64_t >::SimpleScalarTranslator() : ScalarTranslator( 8, ScalarTypes::Float64 ) {}

template< class T >
Helium::Reflect::SimpleScalarTranslator<T>::SimpleScalarTranslator()
{
	HELIUM_COMPILE_ASSERT( false );
}

template< class T >
void Helium::Reflect::SimpleScalarTranslator<T>::Construct( Pointer pointer )
{
	Translator::DefaultConstruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleScalarTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleScalarTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy< T >( src, dest, flags );
}

template< class T >
bool Helium::Reflect::SimpleScalarTranslator<T>::Equals( Pointer a, Pointer b )
{
	return DefaultEquals< T >( a, b );
}

template< class T >
void Helium::Reflect::SimpleScalarTranslator<T>::Accept( Pointer pointer, Visitor& visitor )
{
	DefaultAccept< T >( pointer, visitor );
}

template< class T >
void Helium::Reflect::SimpleScalarTranslator<T>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	DefaultPrint< T >( pointer, string, identifier );
}

template< class T >
void Helium::Reflect::SimpleScalarTranslator<T>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	DefaultParse< T >( string, pointer, resolver, raiseChanged );
}

template<>
inline void Helium::Reflect::SimpleScalarTranslator<uint8_t>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	uint16_t v = pointer.As<uint8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleScalarTranslator<uint8_t>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	uint16_t v = 0;
	str >> v;
	
	pointer.As<uint8_t>() = static_cast<uint8_t>( v );
}

template<>
inline void Helium::Reflect::SimpleScalarTranslator<int8_t>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	int16_t v = pointer.As<int8_t>();

	std::stringstream str;
	str << v;
	string = str.str().c_str();
}

template<>
inline void Helium::Reflect::SimpleScalarTranslator<int8_t>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	std::stringstream str ( string.GetData() );
	int16_t v = 0;
	str >> v;
	
	pointer.As<int8_t>() = static_cast<int8_t>( v );
}

//////////////////////////////////////////////////////////////////////////

template< class T >
Helium::Reflect::SimpleStructureTranslator<T>::SimpleStructureTranslator()
	: StructureTranslator( sizeof( T ) )
{
}

template< class T >
void Helium::Reflect::SimpleStructureTranslator<T>::Construct( Pointer pointer )
{
	Translator::DefaultConstruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleStructureTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct< T >( pointer );
}

template< class T >
void Helium::Reflect::SimpleStructureTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	const Structure* structure = Reflect::GetStructure< T >();
	structure->Copy( src.m_Address, src.m_Object, dest.m_Address, dest.m_Object );

	if ( flags & CopyFlags::Notify && dest.m_Object )
	{
		dest.m_Object->RaiseChanged( dest.m_Field ); 
	}
}

template< class T >
bool Helium::Reflect::SimpleStructureTranslator<T>::Equals( Pointer a, Pointer b )
{
	const Structure* structure = Reflect::GetStructure< T >();
	return structure->Equals( a.m_Address, a.m_Object, b.m_Address, b.m_Object );

}

template< class T >
void Helium::Reflect::SimpleStructureTranslator<T>::Accept( Pointer pointer, Visitor& visitor )
{
	const Structure* structure = Reflect::GetStructure< T >();
	structure->Visit( pointer.m_Address, pointer.m_Object, visitor );
}

template< class T >
const Helium::Reflect::Structure* Helium::Reflect::SimpleStructureTranslator<T>::GetStructure() const
{
	return Reflect::GetStructure< T >();
}

//////////////////////////////////////////////////////////////////////////

template< class T >
Helium::Reflect::PointerTranslator<T>::PointerTranslator()
	: ScalarTranslator( sizeof( StrongPtr< T > ), ScalarTypes::String )
{
}

template< class T >
void Helium::Reflect::PointerTranslator<T>::Construct( Pointer pointer )
{
	DefaultConstruct< StrongPtr< T > >( pointer );
}

template< class T >
void Helium::Reflect::PointerTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct< StrongPtr< T > >( pointer );
}

template< class T >
void Helium::Reflect::PointerTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	if ( flags & CopyFlags::Shallow )
	{
		DefaultCopy< StrongPtr< T > >( src, dest, flags );
	}
	else
	{
		StrongPtr< T >& srcPtr ( src.As< StrongPtr< T > >() );
		StrongPtr< T >& destPtr ( dest.As< StrongPtr< T > >() );
		if ( srcPtr.ReferencesObject() )
		{
			destPtr = AssertCast< T >( srcPtr->Clone() );
		}
		else
		{
			destPtr = NULL;
		}
	}

	if ( flags & CopyFlags::Notify && dest.m_Object )
	{
		dest.m_Object->RaiseChanged( dest.m_Field ); 
	}
}

template< class T >
bool Helium::Reflect::PointerTranslator<T>::Equals( Pointer a, Pointer b )
{
	if ( DefaultEquals< StrongPtr< T > >( a, b ) )
	{
		return true;
	}
	
	return a.As< StrongPtr< T > >()->Equals( b.As< StrongPtr< T > >() );
}

template< class T >
void Helium::Reflect::PointerTranslator<T>::Accept( Pointer pointer, Visitor& visitor )
{
	if ( !visitor.VisitPointer( pointer.As< ObjectPtr >() ) )
	{
		return;
	}

	pointer.As< StrongPtr< T > >()->Accept( visitor );
}

template< class T >
void Helium::Reflect::PointerTranslator<T>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier)
{
	if ( identifier )
	{
		Name name;
		identifier->Identify( pointer.As< StrongPtr< T > >(), name );
		string = name.Get();
	}
}

template< class T >
void Helium::Reflect::PointerTranslator<T>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	if ( resolver )
	{
		resolver->Resolve( Name( string ), pointer.As< StrongPtr< T > >() );

		if ( raiseChanged && pointer.m_Object )
		{
			pointer.m_Object->RaiseChanged( pointer.m_Field ); 
		}
	}
}

//////////////////////////////////////////////////////////////////////////

template< class T >
Helium::Reflect::EnumerationTranslator<T>::EnumerationTranslator()
	: ScalarTranslator( sizeof( T ), ScalarTypes::String )
{
}

template< class T >
void Helium::Reflect::EnumerationTranslator<T>::Construct( Pointer pointer )
{
	DefaultConstruct<T>( pointer );
}

template< class T >
void Helium::Reflect::EnumerationTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct<T>( pointer );
}

template< class T >
void Helium::Reflect::EnumerationTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags )
{
	DefaultCopy<T>( src, dest, flags );
}

template< class T >
bool Helium::Reflect::EnumerationTranslator<T>::Equals( Pointer a, Pointer b )
{
	return DefaultEquals<T>( a, b );
}

template< class T >
void Helium::Reflect::EnumerationTranslator<T>::Accept( Pointer pointer, Visitor& visitor )
{
	DefaultAccept<T>( pointer, visitor );
}

template< class T >
void Helium::Reflect::EnumerationTranslator<T>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier)
{
	const Enumeration* enumeration = GetEnumeration< T >();

	uint32_t value = 0;
	value = static_cast< uint32_t >( pointer.As< T >() );
	HELIUM_COMPILE_ASSERT( sizeof( T::Enum ) == sizeof( value ) );

	tstring str;
	enumeration->GetString( value, str );
	string = str.c_str();
}

template< class T >
void Helium::Reflect::EnumerationTranslator<T>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	const Enumeration* enumeration = GetEnumeration< T >();
	tstring str = string.GetData();

	uint32_t value = 0;
	enumeration->GetValue( str, value );
	pointer.As< T >() = static_cast< T::Enum >( value );
	HELIUM_COMPILE_ASSERT( sizeof( T::Enum ) == sizeof( value ) );

	if ( raiseChanged && pointer.m_Object )
	{
		pointer.m_Object->RaiseChanged( pointer.m_Field ); 
	}
}
