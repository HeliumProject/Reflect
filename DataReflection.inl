#if 0

void TypeIDData::ConnectData(void* data)
{
    m_Data.Connect( data );
}

bool TypeIDData::Set(Data* data, uint32_t flags)
{
    const TypeIDData* rhs = SafeCast<TypeIDData>(data);
    if (!rhs)
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

bool TypeIDData::Equals(Object* object)
{
    const TypeIDData* rhs = SafeCast<TypeIDData>(object);
    if (!rhs)
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

void TypeIDData::Serialize(ArchiveBinary& archive)
{
    const Type* type = *m_Data;
    uint32_t crc = type ? Crc32( type->m_Name ) : BeginCrc32();
    archive.GetStream().Write(&crc); 
}

void TypeIDData::Deserialize(ArchiveBinary& archive)
{
    uint32_t crc;
    archive.GetStream().Read(&crc);

    const Type* type = Registry::GetInstance()->GetType( crc );
    if ( type )
    {
        *m_Data = type;
    }
}

void TypeIDData::Serialize(ArchiveXML& archive)
{
    const Type* type = *m_Data;
    if ( type )
    {
        //archive.GetStream() << "<![CDATA[" << type->m_Name << "]]>";
        archive.WriteString(type->m_Name);
    }
}

void TypeIDData::Deserialize(ArchiveXML& archive)
{
//     std::streamsize size = archive.GetStream().ElementsAvailable(); 
//     tstring str;
//     str.resize( (size_t)size );
//     archive.GetStream().ReadBuffer(const_cast<tchar_t*>(str.c_str()), size);

    tstring str;
    archive.ReadString(str);

    const Type* type = Registry::GetInstance()->GetType( str.c_str() );
    if ( type )
    {
        *m_Data = type;
    }
}

void EnumerationData::ConnectData(void* data)
{
    m_Data.Connect( data );
}

bool EnumerationData::Set(Data* src, uint32_t flags)
{
    if (GetClass() != src->GetClass())
    {
        return false;
    }

    const EnumerationData* rhs = static_cast<const EnumerationData*>(src);

    *m_Data = *rhs->m_Data;

    return true;
}

bool EnumerationData::Equals(Object* object)
{
    const EnumerationData* rhs = SafeCast< EnumerationData >( object );
    
    if (!rhs)
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

void EnumerationData::Serialize(ArchiveBinary& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring label;

    if (enumeration)
    {
        if (!enumeration->GetElementName(*m_Data, label))
        {
            throw Reflect::TypeInformationException( TXT( "Unable to serialize enumeration '%s', value %d" ), enumeration->m_Name, *m_Data );
        }
    }

    archive.GetStream().WriteString( label ); 

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

void EnumerationData::Deserialize(ArchiveBinary& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring str;
    archive.GetStream().ReadString( str );
    if (enumeration && !enumeration->GetElementValue(str, *m_Data))
    {
        Log::Debug( TXT( "Unable to deserialize %s::%s, discarding\n" ), enumeration->m_Name, str.c_str() );
    }
    else
    {
        m_String = str;
    }

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

void EnumerationData::Serialize(ArchiveXML& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring label;
    if (enumeration)
    {
        if (!enumeration->GetElementName(*m_Data, label))
        {
            throw Reflect::TypeInformationException( TXT( "Unable to serialize enumeration '%s', value %d" ), enumeration->m_Name, *m_Data );
        }
    }

    archive.GetStream() << label;

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

void EnumerationData::Deserialize(ArchiveXML& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring buf;
    archive.GetStream() >> buf;
    if (!buf.empty())
    {
        if (enumeration && !enumeration->GetElementValue(buf, *m_Data))
        {
            Log::Debug( TXT( "Unable to deserialize %s::%s, discarding\n" ), enumeration->m_Name, buf.c_str() );
        }
        else
        {
            m_String = buf;
        }
    }

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

tostream& EnumerationData::operator>>(tostream& stream) const
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring label;
    if (enumeration && !enumeration->GetElementName(*m_Data, label))
    {
        // something is amiss, we should be guaranteed serialization of enum elements
        HELIUM_BREAK();
    }

    stream << label;

    return stream;
}

tistream& EnumerationData::operator<<(tistream& stream)
{
    tstring buf;
    stream >> buf;

    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    if ( !buf.empty() && enumeration )
    {
        enumeration->GetElementValue(buf, *m_Data);

        if ( m_Instance && m_Field && m_Field->m_Composite->GetReflectionType() == ReflectionTypes::Class )
        {
            Object* object = static_cast< Object* >( m_Instance );
            object->RaiseChanged( m_Field );
        }
    }

    return stream;
}

void BitfieldData::Serialize(ArchiveBinary& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    if (enumeration)
    {
        std::vector< tstring > strs;
        if (!enumeration->GetBitfieldStrings( *m_Data, strs ))
        {
            throw Reflect::TypeInformationException( TXT( "Unable to serialize bitfield '%s', value %d" ), enumeration->m_Name, *m_Data );
        }

        uint32_t count = (uint32_t)strs.size();
        archive.GetStream().Write(&count); 

        std::vector< tstring >::const_iterator itr = strs.begin();
        std::vector< tstring >::const_iterator end = strs.end();
        for ( ; itr != end; ++itr )
        {
            archive.GetStream().WriteString( *itr );
        }
    }

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

void BitfieldData::Deserialize(ArchiveBinary& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    uint32_t count = 0;
    archive.GetStream().Read(&count); 

    std::vector< tstring > strs;
    strs.reserve( count );
    while ( count-- > 0 )
    {
        tstring str;
        archive.GetStream().ReadString( str );
        strs.push_back( str );
    }

    tstring str;
    std::vector< tstring >::const_iterator itr = strs.begin();
    std::vector< tstring >::const_iterator end = strs.end();
    for ( ; itr != end; ++itr )
    {
        if (itr != strs.begin())
        {
            str += TXT("|");
        }

        str += *itr;
    }

    if (enumeration && !enumeration->GetBitfieldValue(strs, *m_Data))
    {
        Log::Debug( TXT( "Unable to deserialize bitfield %s values '%s'\n" ), enumeration->m_Name, str.c_str() );
    }
    else
    {
        m_String = str;
    }

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

void BitfieldData::Serialize(ArchiveXML& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring str;
    if (enumeration)
    {
        if (!enumeration->GetBitfieldString( *m_Data, str ))
        {
            throw Reflect::TypeInformationException( TXT( "Unable to serialize bitfield '%s', value %d" ), enumeration->m_Name, *m_Data );
        }
    }

    archive.GetStream() << str;

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

void BitfieldData::Deserialize(ArchiveXML& archive)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring buf;
    archive.GetStream() >> buf;

    if (enumeration && !enumeration->GetBitfieldValue(buf, *m_Data))
    {
        Log::Debug( TXT( "Unable to deserialize bitfield %s values '%s'\n" ), enumeration->m_Name, buf );
    }
    else
    {
        m_String = buf;
    }

    if (enumeration == NULL)
    {
        throw Reflect::TypeInformationException( TXT( "Missing type information" ) );
    }
}

tostream& BitfieldData::operator>>(tostream& stream) const
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring str;
    if ( enumeration && !enumeration->GetBitfieldString( *m_Data, str ) )
    {
        // something is amiss, we should be guaranteed serialization of enum elements
        HELIUM_BREAK();
    }

    stream << str;

    return stream;
}

tistream& BitfieldData::operator<<(tistream& stream)
{
    const Enumeration* enumeration = NULL;
    
    if ( m_Field )
    {
        enumeration = ReflectionCast< Enumeration >( m_Field->m_Type );
    }
    else
    {
        HELIUM_BREAK(); // not really supported yet
    }

    tstring buf;
    stream >> buf;

    if ( enumeration )
    {
        enumeration->GetBitfieldValue( buf, *m_Data );
    }

    return stream;
}

void PointerData::ConnectData(void* data)
{
    m_Data.Connect( data );
}

bool PointerData::Set(Data* data, uint32_t flags)
{
    const PointerData* rhs = SafeCast<PointerData>(data);
    if (!rhs)
    {
        return false;
    }

    if (flags & DataFlags::Shallow)
    {
        *m_Data = *rhs->m_Data;
    }
    else
    {
        if ( rhs->m_Data->HasLinkIndex() )
        {
            m_Data->SetLinkIndex( rhs->m_Data->GetLinkIndex() );
        }
        else
        {
            *m_Data = (*rhs->m_Data).ReferencesObject() ? (*rhs->m_Data)->Clone() : NULL;
        }
    }

    return true;
}

bool PointerData::Equals(Object* object)
{
    const PointerData* rhs = SafeCast<PointerData>(object);
    
    if (!rhs)
    {
        return false;
    }

    // if the pointers are equal we are done
    if ( *m_Data == *rhs->m_Data )
    {
        return true;
    }
    // if they are not equal but one is null we are done
    else if ( !(*m_Data).ReferencesObject() || !(*rhs->m_Data).ReferencesObject() )
    {
        return false;
    }

    return (*m_Data)->Equals( *rhs->m_Data );
}

void PointerData::Accept(Visitor& visitor)
{
    if ( !visitor.VisitPointer( *m_Data ) )
    {
        return;
    }

    if ( *m_Data )
    {
        (*m_Data)->Accept( visitor );
    }
}

void PointerData::Serialize(ArchiveBinary& archive)
{
    Serialize<ArchiveBinary>( archive );
}

void PointerData::Deserialize(ArchiveBinary& archive)
{
    Deserialize<ArchiveBinary>( archive );
}

void PointerData::Serialize(ArchiveXML& archive)
{
    Serialize<ArchiveXML>( archive );
}

void PointerData::Deserialize(ArchiveXML& archive)
{
    Deserialize<ArchiveXML>( archive );
}

template< class ArchiveT >
void PointerData::Serialize(ArchiveT& archive)
{
    archive.SerializeInstance( *m_Data );
}

template< class ArchiveT >
void PointerData::Deserialize(ArchiveT& archive)
{
    *m_Data = NULL;

    archive.DeserializeInstance( *m_Data );
}

void StructureData::ConnectData(void* data)
{
    const Structure* structure = NULL;
    
    if ( data )
    {
        structure = ReflectionCast< Structure >( m_Field->m_Type );
        HELIUM_ASSERT( structure );
    }

    m_Data.Connect( data, structure ? structure->m_Size : 0x0 );
}

bool StructureData::Set(Data* data, uint32_t flags)
{
    StructureData* rhs = SafeCast<StructureData>(data);
    if (!rhs)
    {
        return false;
    }

    const Structure* structure = ReflectionCast< Structure >( m_Field->m_Type );
    HELIUM_ASSERT( structure );

    structure->Copy( rhs->m_Data.Get( structure->m_Size ), m_Data.Get( structure->m_Size ) );
    return true;
}

bool StructureData::Equals(Object* object)
{
    StructureData* rhs = SafeCast<StructureData>(object);
    if (!rhs)
    {
        return false;
    }

    const Structure* structure = ReflectionCast< Structure >( m_Field->m_Type );
    HELIUM_ASSERT( structure );

    return structure->Equals( rhs->m_Data.Get( structure->m_Size ), m_Data.Get( structure->m_Size ) );
}

void StructureData::Accept(Visitor& visitor)
{
    const Structure* structure = ReflectionCast< Structure >( m_Field->m_Type );
    HELIUM_ASSERT( structure );

    structure->Visit( m_Data.Get( structure->m_Size ), visitor );
}

void StructureData::Serialize(ArchiveBinary& archive)
{
    const Structure* structure = ReflectionCast< Structure >( m_Field->m_Type );
    HELIUM_ASSERT( structure );

    archive.SerializeInstance( m_Data.Get( structure->m_Size ), structure );
}

void StructureData::Deserialize(ArchiveBinary& archive)
{
#pragma TODO("This is not lossless, data is being discarded")
    if ( m_Field )
    {
        const Structure* structure = ReflectionCast< Structure >( m_Field->m_Type );
        HELIUM_ASSERT( structure );

        archive.DeserializeInstance( m_Data.Get( structure->m_Size ), structure );
    }
}

void StructureData::Serialize(ArchiveXML& archive)
{
    const Structure* structure = ReflectionCast< Structure >( m_Field->m_Type );
    HELIUM_ASSERT( structure );

    archive.SerializeInstance( m_Data.Get( structure->m_Size ), structure );
}

void StructureData::Deserialize(ArchiveXML& archive)
{
#pragma TODO("This is not lossless, data is being discarded")
    if ( m_Field )
    {
        const Structure* structure = ReflectionCast< Structure >( m_Field->m_Type );
        HELIUM_ASSERT( structure );

        archive.DeserializeInstance( m_Data.Get( structure->m_Size ), structure );
    }
}

void Helium::Reflect::StructureData::AllocateForArrayEntry( void *instance, const Field *field )
{
    
    const Structure* structure = ReflectionCast< Structure >( field->m_Type );
    HELIUM_ASSERT( structure );

    structure->Copy(structure->m_Default, m_Data.Get(structure->m_Size));
    m_Instance = instance;
    m_Field = field;

}

#endif