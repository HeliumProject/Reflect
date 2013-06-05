const Helium::Reflect::Type* Helium::Reflect::Registry::GetType( const tchar_t* name ) const
{
    uint32_t crc = Crc32( name );
    return GetType( crc );
}

const Helium::Reflect::Structure* Helium::Reflect::Registry::GetStructure( const tchar_t* name ) const
{
    uint32_t crc = Crc32( name );
    return GetStructure( crc );
}

const Helium::Reflect::Class* Helium::Reflect::Registry::GetClass( const tchar_t* name ) const
{
    uint32_t crc = Crc32( name );
    return GetClass( crc );
}

const Helium::Reflect::Enumeration* Helium::Reflect::Registry::GetEnumeration( const tchar_t* name ) const
{
    uint32_t crc = Crc32( name );
    return GetEnumeration( crc );
}
