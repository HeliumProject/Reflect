template< class T >
void Helium::Reflect::MetaEnum::Create( MetaEnum const*& pointer, const char* name )
{
	MetaEnum* type = new MetaEnum();
	pointer = type;

	type->m_Size = sizeof(T);
	type->m_Name = name;

	T::PopulateMetaType( *type );
}

bool Helium::Reflect::MetaEnum::IsFlagSet(uint32_t value, uint32_t flag)
{
	return ((value & flag) == flag);
}

void Helium::Reflect::MetaEnum::SetFlags(uint32_t& value, uint32_t flags)
{
	value |= flags;
}

template< class EnumerationT >
Helium::Reflect::MetaEnumRegistrar< EnumerationT >::MetaEnumRegistrar(const char* name)
	: MetaTypeRegistrar( name )
{
	HELIUM_ASSERT( EnumerationT::s_Enumeration == NULL );
	MetaTypeRegistrar::AddToList( RegistrarTypes::MetaEnum, this );
}

template< class EnumerationT >
Helium::Reflect::MetaEnumRegistrar< EnumerationT >::~MetaEnumRegistrar()
{
	Unregister();
	MetaTypeRegistrar::RemoveFromList( RegistrarTypes::MetaEnum, this );
}

template< class EnumerationT >
void Helium::Reflect::MetaEnumRegistrar< EnumerationT >::Register()
{
	if( EnumerationT::s_Enumeration == NULL )
	{
		AddTypeToRegistry( EnumerationT::CreateEnumeration() );
	}
}

template< class EnumerationT >
void Helium::Reflect::MetaEnumRegistrar< EnumerationT >::Unregister()
{
	if( EnumerationT::s_Enumeration != NULL )
	{
		RemoveTypeFromRegistry( EnumerationT::s_Enumeration );
		EnumerationT::s_Enumeration = NULL;
	}
}