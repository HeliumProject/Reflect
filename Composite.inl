//
// Composite
//

template< class StructureT >
void Helium::Reflect::Composite::Create( Composite const*& pointer, const tchar_t* name, const tchar_t* baseName )
{
	Composite* type = Composite::Create();
	pointer = type;

	// populate reflection information
	Composite::Create< StructureT >( name, baseName, &StructureT::PopulateComposite, type );

	type->m_Default = new StructureT;
}

template< class CompositeT >
void Helium::Reflect::Composite::Create( const tchar_t* name, const tchar_t* baseName, PopulateCompositeFunc populate, Composite* info )
{
	// the size
	info->m_Size = sizeof( CompositeT );

	// the name of this composite
	info->m_Name = name;

	// lookup base class
	if ( baseName )
	{
		info->m_Base = Reflect::Registry::GetInstance()->GetClass( baseName );

		// if you hit this break your base class is not registered yet!
		HELIUM_ASSERT( info->m_Base );

		// populate base classes' derived class list (unregister will remove it)
		info->m_Base->AddDerived( info );
	}

	// c++ can give us the address of base class static functions,
	//  so check each base class to see if this is really a base class enumerate function
	bool baseAccept = false;
	{
		const Reflect::Composite* base = info->m_Base;
		while ( !baseAccept && base )
		{
			if (base)
			{
				baseAccept = base->m_Populate && base->m_Populate == populate;
				base = base->m_Base;
			}
			else
			{
				HELIUM_BREAK(); // if you hit this break your base class is not registered yet!
				baseName = NULL;
			}
		}
	}

	// if our enumerate function isn't one from a base class
	if ( !baseAccept )
	{
		// the accept function will populate our field data
		info->m_Populate = populate;
	}

	// populate reflection information
	if ( info->m_Populate )
	{
		info->m_Populate( *info );
	}
}

template< class CompositeT, typename FieldT >
const Helium::Reflect::Field* Helium::Reflect::Composite::FindField( FieldT CompositeT::* pointerToMember ) const
{
	return FindFieldByOffset( Reflect::Composite::GetOffset<CompositeT, FieldT>( pointerToMember ) );
}

template < class CompositeT, class FieldT >
uint32_t Helium::Reflect::Composite::GetOffset( FieldT CompositeT::* field )
{
	return (uint32_t) (uintptr_t) &( ((CompositeT*)NULL)->*field); 
}

template < class T, size_t N >
uint32_t Helium::Reflect::Composite::GetArrayCount( T (&/* array */)[N] )
{
	return N;
}

template < class T >
uint32_t Helium::Reflect::Composite::GetCount( std::false_type /* is_array */ )
{
	return 1;
}

template < class T >
uint32_t Helium::Reflect::Composite::GetCount( std::true_type /* is_array */ )
{
	T temp;
	return GetArrayCount( temp );
}

template < class T >
Helium::Reflect::Data* Helium::Reflect::Composite::AllocateData( std::false_type /* is_array */ )
{
	return Reflect::AllocateData< T >();
}

template < class T >
Helium::Reflect::Data* Helium::Reflect::Composite::AllocateData( std::true_type /* is_array */ )
{
	return Reflect::AllocateData< std::remove_extent< T >::type >();
}

template < class CompositeT, class FieldT >
Helium::Reflect::Field* Helium::Reflect::Composite::AddField( FieldT CompositeT::* field, const tchar_t* name, uint32_t flags, Data* data )
{
	if ( data == NULL )
	{
		data = AllocateData<FieldT>( std::is_array< FieldT >() );
	}

	return AddField( name, sizeof(FieldT), GetCount< FieldT >( std::is_array< FieldT >() ), GetOffset(field), flags, data );
}

//
// StructureRegistrar
//

template< class StructureT, class BaseT >
Helium::Reflect::StructureRegistrar< StructureT, BaseT >::StructureRegistrar(const tchar_t* name)
	: TypeRegistrar( name )
{
	HELIUM_ASSERT( StructureT::s_Composite == NULL );
	TypeRegistrar::AddToList( RegistrarTypes::Structure, this );
}

template< class StructureT, class BaseT >
Helium::Reflect::StructureRegistrar< StructureT, BaseT >::~StructureRegistrar()
{
	Unregister();
	TypeRegistrar::RemoveFromList( RegistrarTypes::Structure, this );
}

template< class StructureT, class BaseT >
void Helium::Reflect::StructureRegistrar< StructureT, BaseT >::Register()
{
	if ( StructureT::s_Composite == NULL )
	{
		BaseT::s_Registrar.Register();
		AddTypeToRegistry( StructureT::CreateComposite() );
	}
}

template< class StructureT, class BaseT >
void Helium::Reflect::StructureRegistrar< StructureT, BaseT >::Unregister()
{
	if ( StructureT::s_Composite != NULL )
	{
		RemoveTypeFromRegistry( StructureT::s_Composite );
		StructureT::s_Composite = NULL;
	}
}

template< class StructureT >
Helium::Reflect::StructureRegistrar< StructureT, void >::StructureRegistrar(const tchar_t* name)
	: TypeRegistrar( name )
{
	HELIUM_ASSERT( StructureT::s_Composite == NULL );
	TypeRegistrar::AddToList( RegistrarTypes::Structure, this );
}

template< class StructureT >
Helium::Reflect::StructureRegistrar< StructureT, void >::~StructureRegistrar()
{
	Unregister();
	TypeRegistrar::RemoveFromList( RegistrarTypes::Structure, this );
}

template< class StructureT >
void Helium::Reflect::StructureRegistrar< StructureT, void >::Register()
{
	if ( StructureT::s_Composite == NULL )
	{
		AddTypeToRegistry( StructureT::CreateComposite() );
	}
}

template< class StructureT >
void Helium::Reflect::StructureRegistrar< StructureT, void >::Unregister()
{
	if ( StructureT::s_Composite != NULL )
	{
		RemoveTypeFromRegistry( StructureT::s_Composite );
		StructureT::s_Composite = NULL;
	}
}
