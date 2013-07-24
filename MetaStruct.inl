template< class StructureT >
void Helium::Reflect::MetaStruct::Create( MetaStruct const*& pointer, const char* name, const char* baseName )
{
	MetaStruct* type = MetaStruct::Create();
	pointer = type;

	// populate reflection information
	MetaStruct::Create< StructureT >( name, baseName, &StructureT::PopulateStructure, type );

	type->m_Default = new StructureT;
}

template< class StructureT >
void Helium::Reflect::MetaStruct::Create( const char* name, const char* baseName, PopulateCompositeFunc populate, MetaStruct* info )
{
	// the size
	info->m_Size = sizeof( StructureT );

	// the name of this composite
	info->m_Name = name;

	// lookup base class
	if ( baseName )
	{
		info->m_Base = Reflect::Registry::GetInstance()->GetStructure( baseName );

		// if you hit this break your base class is not registered yet!
		HELIUM_ASSERT( info->m_Base );

		// populate base classes' derived class list (unregister will remove it)
		info->m_Base->AddDerived( info );
	}

	// c++ can give us the address of base class static functions,
	//  so check each base class to see if this is really a base class enumerate function
	bool baseAccept = false;
	{
		const Reflect::MetaStruct* base = info->m_Base;
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

template< class StructureT, typename FieldT >
const Helium::Reflect::Field* Helium::Reflect::MetaStruct::FindField( FieldT StructureT::* pointerToMember ) const
{
	return FindFieldByOffset( Reflect::MetaStruct::GetOffset<StructureT, FieldT>( pointerToMember ) );
}

template < class StructureT, class FieldT >
uint32_t Helium::Reflect::MetaStruct::GetOffset( FieldT StructureT::* field )
{
	return (uint32_t) (uintptr_t) &( ((StructureT*)NULL)->*field); 
}

template < class T, size_t N >
uint32_t Helium::Reflect::MetaStruct::GetArrayCount( T (&/* array */)[N] )
{
	return N;
}

template < class T >
uint32_t Helium::Reflect::MetaStruct::GetCount( std::false_type /* is_array */ )
{
	return 1;
}

template < class T >
uint32_t Helium::Reflect::MetaStruct::GetCount( std::true_type /* is_array */ )
{
	T temp;
	return GetArrayCount( temp );
}

template < class T >
const Helium::Reflect::MetaType* Helium::Reflect::MetaStruct::DeduceKeyType( std::false_type /* is_array */ )
{
	return Reflect::DeduceKeyType< T >();
}

template < class T >
const Helium::Reflect::MetaType* Helium::Reflect::MetaStruct::DeduceKeyType( std::true_type /* is_array */ )
{
	return Reflect::DeduceKeyType< typename std::remove_extent< T >::type >();
}

template < class T >
const Helium::Reflect::MetaType* Helium::Reflect::MetaStruct::DeduceValueType( std::false_type /* is_array */ )
{
	return Reflect::DeduceValueType< T >();
}

template < class T >
const Helium::Reflect::MetaType* Helium::Reflect::MetaStruct::DeduceValueType( std::true_type /* is_array */ )
{
	return Reflect::DeduceValueType< typename std::remove_extent< T >::type >();
}

template < class T >
Helium::Reflect::Translator* Helium::Reflect::MetaStruct::AllocateTranslator( std::false_type /* is_array */ )
{
	return Reflect::AllocateTranslator< T >();
}

template < class T >
Helium::Reflect::Translator* Helium::Reflect::MetaStruct::AllocateTranslator( std::true_type /* is_array */ )
{
	return Reflect::AllocateTranslator< typename std::remove_extent< T >::type >();
}

template < class StructureT, class FieldT >
Helium::Reflect::Field* Helium::Reflect::MetaStruct::AddField( FieldT StructureT::* field, const char* name, uint32_t flags, Translator* translator )
{
	Field* f = AddField();
	f->m_Name = name;
	f->m_Size = sizeof(FieldT);
	f->m_Count = GetCount< FieldT >( std::is_array< FieldT >() );
	f->m_Offset = GetOffset(field);
	f->m_Flags = flags;
	f->m_KeyType = DeduceKeyType<FieldT>( std::is_array< FieldT >() );
	f->m_ValueType = DeduceValueType<FieldT>( std::is_array< FieldT >() );
	f->m_Translator = translator ? translator : AllocateTranslator<FieldT>( std::is_array< FieldT >() );
	return f;
}

//
// MetaStructRegistrar
//

template< class StructureT, class BaseT >
Helium::Reflect::MetaStructRegistrar< StructureT, BaseT >::MetaStructRegistrar(const char* name)
	: MetaTypeRegistrar( name )
{
	HELIUM_ASSERT( StructureT::s_Structure == NULL );
	MetaTypeRegistrar::AddToList( RegistrarTypes::MetaStruct, this );
}

template< class StructureT, class BaseT >
Helium::Reflect::MetaStructRegistrar< StructureT, BaseT >::~MetaStructRegistrar()
{
	Unregister();
	MetaTypeRegistrar::RemoveFromList( RegistrarTypes::MetaStruct, this );
}

template< class StructureT, class BaseT >
void Helium::Reflect::MetaStructRegistrar< StructureT, BaseT >::Register()
{
	if ( StructureT::s_Structure == NULL )
	{
		BaseT::s_Registrar.Register();
		AddTypeToRegistry( StructureT::CreateStructure() );
	}
}

template< class StructureT, class BaseT >
void Helium::Reflect::MetaStructRegistrar< StructureT, BaseT >::Unregister()
{
	if ( StructureT::s_Structure != NULL )
	{
		RemoveTypeFromRegistry( StructureT::s_Structure );
		StructureT::s_Structure = NULL;
	}
}

template< class StructureT >
Helium::Reflect::MetaStructRegistrar< StructureT, void >::MetaStructRegistrar(const char* name)
	: MetaTypeRegistrar( name )
{
	HELIUM_ASSERT( StructureT::s_Structure == NULL );
	MetaTypeRegistrar::AddToList( RegistrarTypes::MetaStruct, this );
}

template< class StructureT >
Helium::Reflect::MetaStructRegistrar< StructureT, void >::~MetaStructRegistrar()
{
	Unregister();
	MetaTypeRegistrar::RemoveFromList( RegistrarTypes::MetaStruct, this );
}

template< class StructureT >
void Helium::Reflect::MetaStructRegistrar< StructureT, void >::Register()
{
	if ( StructureT::s_Structure == NULL )
	{
		AddTypeToRegistry( StructureT::CreateStructure() );
	}
}

template< class StructureT >
void Helium::Reflect::MetaStructRegistrar< StructureT, void >::Unregister()
{
	if ( StructureT::s_Structure != NULL )
	{
		RemoveTypeFromRegistry( StructureT::s_Structure );
		StructureT::s_Structure = NULL;
	}
}
