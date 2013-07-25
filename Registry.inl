const Helium::Reflect::MetaType* Helium::Reflect::Registry::GetType( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetType( crc );
}

const Helium::Reflect::MetaStruct* Helium::Reflect::Registry::GetMetaStruct( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetMetaStruct( crc );
}

const Helium::Reflect::MetaClass* Helium::Reflect::Registry::GetMetaClass( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetMetaClass( crc );
}

const Helium::Reflect::MetaEnum* Helium::Reflect::Registry::GetMetaEnum( const char* name ) const
{
    uint32_t crc = Crc32( name );
    return GetMetaEnum( crc );
}
