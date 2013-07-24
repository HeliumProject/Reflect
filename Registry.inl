const Helium::Reflect::MetaType* Helium::Reflect::Registry::GetType( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetType( crc );
}

const Helium::Reflect::MetaStruct* Helium::Reflect::Registry::GetStructure( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetStructure( crc );
}

const Helium::Reflect::MetaClass* Helium::Reflect::Registry::GetMetaClass( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetMetaClass( crc );
}

const Helium::Reflect::MetaEnum* Helium::Reflect::Registry::GetEnumeration( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetEnumeration( crc );
}
