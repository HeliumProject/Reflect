template< class T >
void Helium::Reflect::Enumeration::Create( Enumeration const*& pointer, const char* name )
{
	Enumeration* type = new Enumeration();
	pointer = type;

	type->m_Size = sizeof(T);
	type->m_Name = name;

	T::EnumerateEnum( *type );
}

bool Helium::Reflect::Enumeration::IsFlagSet(uint32_t value, uint32_t flag)
{
	return ((value & flag) == flag);
}

void Helium::Reflect::Enumeration::SetFlags(uint32_t& value, uint32_t flags)
{
	value |= flags;
}

template< class EnumerationT >
Helium::Reflect::EnumRegistrar< EnumerationT >::EnumRegistrar(const char* name)
	: TypeRegistrar( name )
{
	HELIUM_ASSERT( EnumerationT::s_Enumeration == NULL );
	TypeRegistrar::AddToList( RegistrarTypes::Enumeration, this );
}

template< class EnumerationT >
Helium::Reflect::EnumRegistrar< EnumerationT >::~EnumRegistrar()
{
	Unregister();
	TypeRegistrar::RemoveFromList( RegistrarTypes::Enumeration, this );
}

template< class EnumerationT >
void Helium::Reflect::EnumRegistrar< EnumerationT >::Register()
{
	if( EnumerationT::s_Enumeration == NULL )
	{
		AddTypeToRegistry( EnumerationT::CreateEnumeration() );
	}
}

template< class EnumerationT >
void Helium::Reflect::EnumRegistrar< EnumerationT >::Unregister()
{
	if( EnumerationT::s_Enumeration != NULL )
	{
		RemoveTypeFromRegistry( EnumerationT::s_Enumeration );
		EnumerationT::s_Enumeration = NULL;
	}
}