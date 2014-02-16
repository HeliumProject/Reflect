template< class T >
Helium::Reflect::SimpleScalarTranslator<T>::SimpleScalarTranslator()
{
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
void Helium::Reflect::SimpleScalarTranslator<T>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	DefaultPrint< T >( pointer, string, identifier );
}

template< class T >
void Helium::Reflect::SimpleScalarTranslator<T>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	DefaultParse< T >( string, pointer, resolver, raiseChanged );
}

namespace Helium
{
	namespace Reflect
	{
		template<>
		inline void SimpleScalarTranslator<uint8_t>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
		{
			uint16_t v = pointer.As<uint8_t>();

			std::stringstream str;
			str << v;
			string = str.str().c_str();
		}

		template<>
		inline void SimpleScalarTranslator<uint8_t>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
		{
			std::stringstream str ( string.GetData() );
			uint16_t v = 0;
			str >> v;
			
			pointer.As<uint8_t>() = static_cast<uint8_t>( v );
		}

		template<>
		inline void SimpleScalarTranslator<int8_t>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
		{
			int16_t v = pointer.As<int8_t>();

			std::stringstream str;
			str << v;
			string = str.str().c_str();
		}

		template<>
		inline void SimpleScalarTranslator<int8_t>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
		{
			std::stringstream str ( string.GetData() );
			int16_t v = 0;
			str >> v;
			
			pointer.As<int8_t>() = static_cast<int8_t>( v );
		}
	}
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
	const MetaStruct* structure = Reflect::GetMetaStruct< T >();
	structure->Copy( src.m_Address, src.m_Object, dest.m_Address, dest.m_Object );
	dest.RaiseChanged( flags & CopyFlags::Notify ); 
}

template< class T >
bool Helium::Reflect::SimpleStructureTranslator<T>::Equals( Pointer a, Pointer b )
{
	const MetaStruct* structure = Reflect::GetMetaStruct< T >();
	return structure->Equals( a.m_Address, a.m_Object, b.m_Address, b.m_Object );

}

template< class T >
const Helium::Reflect::MetaStruct* Helium::Reflect::SimpleStructureTranslator<T>::GetMetaStruct() const
{
	return Reflect::GetMetaStruct< T >();
}

//////////////////////////////////////////////////////////////////////////

template< class T >
Helium::Reflect::PointerTranslator<T>::PointerTranslator()
	: ScalarTranslator( sizeof( StrongPtr< T > ), ScalarTypes::String )
{
}

template< class T >
uint32_t Helium::Reflect::PointerTranslator<T>::GetDefaultFlags()
{
	return T::s_DefaultPointerFlags;
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
			destPtr = static_cast< T* >( srcPtr->Clone().Ptr() );
		}
		else
		{
			destPtr = NULL;
		}
	}

	dest.RaiseChanged( flags & CopyFlags::Notify ); 
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
void Helium::Reflect::PointerTranslator<T>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier )
{
	if ( identifier )
	{
		Name name;
		Identify( identifier, pointer, &name );
		string = name.Get();
	}
}

template< class T >
void Helium::Reflect::PointerTranslator<T>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	if ( resolver )
	{
		Resolve( resolver, Name( string ), pointer );
		pointer.RaiseChanged( raiseChanged );
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
void Helium::Reflect::EnumerationTranslator<T>::Print( Pointer pointer, String& string, ObjectIdentifier* identifier)
{
	const MetaEnum* enumeration = GetMetaEnum< T >();

	uint32_t value = 0;
	value = static_cast< uint32_t >( pointer.As< T >() );
	HELIUM_COMPILE_ASSERT( sizeof( typename T::Enum ) == sizeof( value ) );

	std::string str;
	enumeration->GetString( value, str );
	string = str.c_str();
}

template< class T >
void Helium::Reflect::EnumerationTranslator<T>::Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged )
{
	const MetaEnum* enumeration = GetMetaEnum< T >();
	std::string str = string.GetData();

	uint32_t value = 0;
	enumeration->GetValue( str, value );
	pointer.As< T >() = static_cast< typename T::Enum >( value );
	HELIUM_COMPILE_ASSERT( sizeof( typename T::Enum ) == sizeof( value ) );

	pointer.RaiseChanged( raiseChanged ); 
}

