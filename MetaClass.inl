template< class ClassT >
void Helium::Reflect::MetaClass::Create( MetaClass const*& pointer, const char* name, const char* baseName, CreateObjectFunc creator )
{
	MetaClass* type = MetaClass::Create();
	pointer = type;

	// populate reflection information
	MetaStruct::Create< ClassT >( name, baseName, reinterpret_cast< PopulateMetaTypeFunc >( &ClassT::PopulateMetaType ), type );

	// setup factory function
	type->m_Creator = creator;

	// fetch a potential default instance from the composite
	ClassT* instance = static_cast< ClassT* >( type->MetaStruct::m_Default );
	if ( instance )
	{
		type->m_Default = instance;
	}
	else
	{
		// create the default instance
		if ( pointer->m_Creator )
		{
			type->MetaStruct::m_Default = type->m_Default = type->m_Creator();
		}
	}
}

template< class ClassT, class BaseT >
Helium::Reflect::MetaClassRegistrar< ClassT, BaseT >::MetaClassRegistrar(const char* name)
	: MetaTypeRegistrar( name )
{
	HELIUM_ASSERT( ClassT::s_MetaClass == NULL );
	MetaTypeRegistrar::AddToList( RegistrarTypes::MetaClass, this );
}

template< class ClassT, class BaseT >
Helium::Reflect::MetaClassRegistrar< ClassT, BaseT >::~MetaClassRegistrar()
{
	Unregister();
	MetaTypeRegistrar::RemoveFromList( RegistrarTypes::MetaClass, this );
}

template< class ClassT, class BaseT >
void Helium::Reflect::MetaClassRegistrar< ClassT, BaseT >::Register()
{
	if ( ClassT::s_MetaClass == NULL )
	{
		BaseT::s_Registrar.Register();
		AddTypeToRegistry( ClassT::CreateMetaClass() );
	}
}

template< class ClassT, class BaseT >
void Helium::Reflect::MetaClassRegistrar< ClassT, BaseT >::Unregister()
{
	if ( ClassT::s_MetaClass != NULL )
	{
		RemoveTypeFromRegistry( ClassT::s_MetaClass );
		ClassT::s_MetaClass = NULL;
	}
}

template< class ClassT >
Helium::Reflect::MetaClassRegistrar< ClassT, void >::MetaClassRegistrar(const char* name)
	: MetaTypeRegistrar( name )
{
	HELIUM_ASSERT( ClassT::s_MetaClass == NULL );
	MetaTypeRegistrar::AddToList( RegistrarTypes::MetaClass, this );
}

template< class ClassT >
Helium::Reflect::MetaClassRegistrar< ClassT, void >::~MetaClassRegistrar()
{
	Unregister();
	MetaTypeRegistrar::RemoveFromList( RegistrarTypes::MetaClass, this );
}

template< class ClassT >
void Helium::Reflect::MetaClassRegistrar< ClassT, void >::Register()
{
	if ( ClassT::s_MetaClass == NULL )
	{
		AddTypeToRegistry( ClassT::CreateMetaClass() );
	}
}

template< class ClassT >
void Helium::Reflect::MetaClassRegistrar< ClassT, void >::Unregister()
{
	if ( ClassT::s_MetaClass != NULL )
	{
		RemoveTypeFromRegistry( ClassT::s_MetaClass );
		ClassT::s_MetaClass = NULL;
	}
}