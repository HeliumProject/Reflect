#ifdef REFLECT_REFACTOR

void PathData::ConnectData( void* data )
{
    m_Data.Connect( data );
}

bool PathData::Set( Data* src, uint32_t flags )
{
    if ( GetClass() != src->GetClass() )
    {
        return false;
    }

    const PathData* rhs = static_cast< const PathData* >( src );

    *m_Data = *rhs->m_Data;

    return true;
}

bool PathData::Equals( Object* object )
{
    const PathData* rhs = SafeCast< PathData >( object );

    if ( !rhs )
    {
        return false;
    }

    return *rhs->m_Data == *m_Data;
}

void PathData::Serialize( ArchiveBinary& archive )
{
    const tstring& str = m_Data->Get();
    archive.GetStream().WriteString( str ); 
}

void PathData::Deserialize( ArchiveBinary& archive )
{
    tstring str;
    archive.GetStream().ReadString( str );
    m_Data->Set( str );
}

void PathData::Serialize( ArchiveXML& archive )
{
    const tstring& str = m_Data->Get();
    archive.GetStream() << str;
}

void PathData::Deserialize( ArchiveXML& archive )
{
    tstring buf;
    std::streamsize size = archive.GetStream().ElementsAvailable(); 
    buf.resize( (size_t)size );
    archive.GetStream().ReadBuffer( const_cast<tchar_t*>( buf.c_str() ), size );
    m_Data->Set( buf );
}

tostream& PathData::operator>>( tostream& stream ) const
{
    tstring path = m_Data->Get();
    tstring temp;
    bool converted = Helium::ConvertString( path, temp );
    HELIUM_ASSERT( converted );

    stream << temp;
    return stream;
}

tistream& PathData::operator<<( tistream& stream )
{
    tstring str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.resize( (size_t) size );
    stream.read( const_cast<tchar_t*>( str.c_str() ), size );

    if ( !str.empty() )
    {
        m_Data->Set( str );

        if ( m_Instance && m_Field && m_Field->m_Composite->GetReflectionType() == ReflectionTypes::Class )
        {
            Object* object = static_cast< Object* >( m_Instance );
            object->RaiseChanged( m_Field );
        }
    }

    return stream;
}

template< class T >
void SimpleDynamicArrayData< T >::ConnectData(void* data)
{
    m_Data.Connect( data );
}

template< class T >
size_t SimpleDynamicArrayData< T >::GetSize() const
{ 
    return m_Data->GetSize(); 
}

template< class T >
void SimpleDynamicArrayData< T >::SetSize( size_t size )
{
    return m_Data->Resize( size );
}

template< class T >
void SimpleDynamicArrayData< T >::Clear()
{
    return m_Data->Clear();
}

template< class T >
const Class* SimpleDynamicArrayData< T >::GetItemClass() const
{
    return Reflect::GetDataClass< T >();
}

template< class T >
DataPtr SimpleDynamicArrayData< T >::GetItem( size_t at )
{
    return Data::Bind( m_Data->GetElement( at ), m_Instance, m_Field );
}

template< class T >
void SimpleDynamicArrayData< T >::SetItem( size_t at, Data* value )
{
    Data::GetValue( value, m_Data->GetElement( at ) );
}

template< class T >
void SimpleDynamicArrayData< T >::Insert( size_t at, Data* value )
{
    T temp;
    Data::GetValue( value, temp );
    m_Data->Insert( at, temp );
}

template< class T >
void SimpleDynamicArrayData< T >::Remove( size_t at )
{
    m_Data->Remove( at );
}

template< class T >
void SimpleDynamicArrayData< T >::MoveUp( std::set< size_t >& selectedIndices )
{
    std::set< size_t > newSelectedIndices;

    std::set< size_t >::const_iterator itr = selectedIndices.begin();
    std::set< size_t >::const_iterator end = selectedIndices.end();

    for( ; itr != end; ++itr )
    {
        if ( (*itr) == 0 || ( newSelectedIndices.find( (*itr) - 1 ) != newSelectedIndices.end() ) )
        {
            newSelectedIndices.insert( *itr );
            continue;
        }

        Swap( m_Data->GetElement( *itr - 1 ), m_Data->GetElement( *itr ) );

        newSelectedIndices.insert( *itr - 1 );
    }

    selectedIndices = newSelectedIndices;
}

template< class T >
void SimpleDynamicArrayData< T >::MoveDown( std::set< size_t >& selectedIndices )
{
    std::set< size_t > newSelectedIndices;

    std::set< size_t >::const_reverse_iterator itr = selectedIndices.rbegin();
    std::set< size_t >::const_reverse_iterator end = selectedIndices.rend();

    for( ; itr != end; ++itr )
    {
        if ( ( (*itr) == m_Data->GetSize() - 1 ) || ( newSelectedIndices.find( (*itr) + 1 ) != newSelectedIndices.end() ) )
        {
            newSelectedIndices.insert( *itr );
            continue;
        }

        Swap( m_Data->GetElement( *itr + 1 ), m_Data->GetElement( *itr ) );

        newSelectedIndices.insert( *itr + 1 );
    }

    selectedIndices = newSelectedIndices;
}

template< class T >
bool SimpleDynamicArrayData< T >::Set( Data* src, uint32_t flags )
{
    const SimpleDynamicArrayData< T >* rhs = SafeCast< SimpleDynamicArrayData< T > >( src );
    if (!rhs)
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template< class T >
bool SimpleDynamicArrayData< T >::Equals( Object* object )
{
    const SimpleDynamicArrayData< T >* rhs = SafeCast< SimpleDynamicArrayData< T > >( object );
    if (!rhs)
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

template< class T >
void SimpleDynamicArrayData< T >::Serialize( ArchiveBinary& archive )
{
    Reflect::CharStream& stream = archive.GetStream();

    size_t countActual = m_Data->GetSize();
    HELIUM_ASSERT( countActual <= UINT32_MAX );
    uint32_t count = static_cast< uint32_t >( countActual );
    stream.Write( &count );

    for( size_t index = 0; index < countActual; ++index )
    {
        stream.Write( &m_Data->GetElement( index ) );
    }
}

template< class T >
void SimpleDynamicArrayData< T >::Deserialize( ArchiveBinary& archive )
{
    // if we are referring to a real field, clear its contents
    m_Data->Clear();

    Reflect::CharStream& stream = archive.GetStream();

    uint32_t count = 0;
    stream.Read( &count );

    m_Data->Reserve( count );

    uint_fast32_t countFast = count;
    for( uint_fast32_t index = 0; index < countFast; ++index )
    {
        T* pElement = m_Data->New();
        HELIUM_ASSERT( pElement );
        stream.Read( pElement );
    }
}

template< class T >
void SimpleDynamicArrayData< T >::Serialize( ArchiveXML& archive )
{
    archive.GetIndent().Push();

    // foreach datum
    for (size_t i=0; i<m_Data->GetSize(); i++)
    {
        // indent
        archive.GetIndent().Get(archive.GetStream());

        // write
        archive.GetStream() << (*m_Data)[i];

        // newline
        archive.GetStream() << "\n";
    }

    archive.GetIndent().Pop();
}

template< class T >
void SimpleDynamicArrayData< T >::Deserialize( ArchiveXML& archive )
{
    // if we are referring to a real field, clear its contents
    m_Data->Clear();

    T value;
    archive.GetStream().SkipWhitespace(); 

    while (!archive.GetStream().Done())
    {
        // read data
        archive.GetStream() >> value;

        // copy onto vector
        m_Data->Push(value);

        // read to next non-whitespace char
        archive.GetStream().SkipWhitespace(); 
    }
}

template< class T >
tostream& SimpleDynamicArrayData< T >::operator>>( tostream& stream ) const
{
    size_t elementCount = m_Data->GetSize();

    if( elementCount != 0 )
    {
        stream << m_Data->GetElement( 0 );

        for( size_t index = 1; index < elementCount; ++index )
        {
            stream << s_ContainerItemDelimiter;
            stream << m_Data->GetElement( index );
        }
    }

    return stream;
}

template< class T >
tistream& SimpleDynamicArrayData< T >::operator<<( tistream& stream )
{
    m_Data->Clear();

    String str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.Reserve( static_cast< size_t >( size ) );
    str.Resize( static_cast< size_t >( size ) );
    stream.read( &str[ 0 ], size );

    Tokenize< T, T >( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}

//
// Specializations
//

void SerializeStringDynamicArray( ArchiveXML& archive, DynamicArray<String> data )
{
    archive.GetIndent().Push();
    archive.GetIndent().Get(archive.GetStream());

    // start our CDATA section, this prevents XML from parsing its escapes in this cdata section
    archive.GetStream() << TXT("<![CDATA[\n");

    for (size_t i=0; i<data.GetSize(); i++)
    {
        archive.GetIndent().Get(archive.GetStream());

        // output the escape-code free character sequence between double qutoes
        archive.GetStream() << TXT('\"') << (data)[i] << TXT('\"') << s_ContainerItemDelimiter;
    }

    // end our CDATA escape section
    archive.GetIndent().Get(archive.GetStream());
    archive.GetStream() << TXT("]]>\n");
    archive.GetIndent().Pop();
}

void DeserializeStringDynamicArray( ArchiveXML& archive, DynamicArray<String> data )
{
    archive.GetStream().SkipWhitespace(); 
    tstring value;

    while (!archive.GetStream().Done())
    {
        std::getline( archive.GetStream().GetInternal(), value ); 

        size_t start = value.find_first_of('\"');
        size_t end = value.find_last_of('\"');

        // if we found a pair of quotes
        if (start != std::string::npos && end != std::string::npos && start != end)
        {
            // if all we have are open/close quotes, push a blank string
            String* string = data.New();
            HELIUM_ASSERT( string );
            if (start != end-1)
            {
                *string = value.substr( start + 1, end - start - 1).c_str();
            }
        }
        else
        {
            start = value.find_first_not_of( TXT( " \t\n" ) );

            if ( start != std::string::npos )
            {
                HELIUM_VERIFY( data.New( value.substr(start).c_str() ) );
            }
        }

        archive.GetStream().SkipWhitespace(); 
    }
}

template<>
void StringDynamicArrayData::Serialize( ArchiveBinary& archive )
{
    CharStream& stream = archive.GetStream();

    size_t countActual = m_Data->GetSize();
    HELIUM_ASSERT( countActual <= UINT32_MAX );
    uint32_t count = static_cast< uint32_t >( countActual );
    stream.Write( &count );

    for( size_t index = 0; index < countActual; ++index )
    {
        stream.WriteString( m_Data->GetElement( index ) );
    }
}

template<>
void StringDynamicArrayData::Deserialize( ArchiveBinary& archive )
{
    m_Data->Clear();

    CharStream& stream = archive.GetStream();

    uint32_t count = 0;
    stream.Read( &count );

    m_Data->Reserve( count );

    uint_fast32_t countFast = count;
    for( uint32_t index = 0; index < countFast; index++ )
    {
        String* element = m_Data->New();
        HELIUM_ASSERT( element );
        stream.ReadString( *element ); 
    }
}

template<>
void StringDynamicArrayData::Serialize( ArchiveXML& archive )
{
    SerializeStringDynamicArray(archive, *m_Data);
}

template<>
void StringDynamicArrayData::Deserialize( ArchiveXML& archive )
{
    DeserializeStringDynamicArray(archive, *m_Data);
}

template<>
void NameDynamicArrayData::Serialize( ArchiveBinary& archive )
{
    CharStream& stream = archive.GetStream();

    size_t countActual = m_Data->GetSize();
    HELIUM_ASSERT( countActual <= UINT32_MAX );
    uint32_t count = static_cast< uint32_t >( countActual );
    stream.Write( &count );

    for( size_t index = 0; index < countActual; ++index )
    {
        stream.WriteString( *m_Data->GetElement( index ) );
    }
}

template<>
void NameDynamicArrayData::Deserialize( ArchiveBinary& archive )
{
    m_Data->Clear();

    CharStream& stream = archive.GetStream();

    uint32_t count = 0;
    stream.Read( &count );

    m_Data->Reserve( count );

    uint_fast32_t countFast = count;
    for( uint32_t index = 0; index < countFast; index++ )
    {
        Name* element = m_Data->New();
        HELIUM_ASSERT( element );
        String name_as_str;
        stream.ReadString(name_as_str);
        element->Set(name_as_str);
    }
}

template<>
void NameDynamicArrayData::Serialize( ArchiveXML& archive )
{
    DynamicArray<String> names_as_str;
    names_as_str.Resize(m_Data->GetSize());

    for( size_t index = 0; index < m_Data->GetSize(); ++index )
    {
        names_as_str[index] = *m_Data->GetElement( index );
    }

    SerializeStringDynamicArray(archive, names_as_str);
}

template<>
void NameDynamicArrayData::Deserialize( ArchiveXML& archive )
{
    DynamicArray<String> names_as_str;
    DeserializeStringDynamicArray(archive, names_as_str);

    m_Data->Resize(names_as_str.GetSize());
    
    for ( size_t index = 0; index < m_Data->GetSize(); ++index )
    {
        m_Data->GetElement(index).Set(names_as_str.GetElement(index));
    }
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::ConnectData( void* data )
{
    m_Data.Connect( data );
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
size_t SimpleSetData< KeyT, EqualKeyT, AllocatorT >::GetSize() const
{
    return m_Data->GetSize();
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Clear()
{
    return m_Data->Clear();
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
const Class* SimpleSetData< KeyT, EqualKeyT, AllocatorT >::GetItemClass() const
{
    return Reflect::GetDataClass< KeyT >();
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::GetItems( DynamicArray< DataPtr >& items ) const
{
    items.Clear();
    items.Reserve( m_Data->GetSize() );

    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        HELIUM_VERIFY( items.New( Data::Bind( const_cast< KeyT& >( *itr ), m_Instance, m_Field ) ) );
    }
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::AddItem( Data* value )
{
    KeyT dataValue;
    Data::GetValue( value, dataValue );
    m_Data->Insert( dataValue );
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::RemoveItem( Data* value )
{
    KeyT dataValue;
    Data::GetValue( value, dataValue );
    m_Data->Remove( dataValue );
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
bool SimpleSetData< KeyT, EqualKeyT, AllocatorT >::ContainsItem( Data* value ) const
{
    KeyT dataValue;
    Data::GetValue( value, dataValue );
    return m_Data->Find( dataValue ) != m_Data->End();
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
bool SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Set( Data* src, uint32_t flags )
{
    const SetDataT* rhs = SafeCast< SetDataT >( src );
    if ( !rhs )
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
bool SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Equals( Object* object )
{
    const SetDataT* rhs = SafeCast< SetDataT >( object );
    if ( !rhs )
    {
        return false;
    }

    if ( m_Data->GetSize() != rhs->m_Data->GetSize() )
    {
        return false;
    }

    const DataType& rhsData = *m_Data;

    DataType::ConstIterator itrLHS = m_Data->Begin();
    DataType::ConstIterator endLHS = m_Data->End();
    DataType::ConstIterator endRHS = rhsData.End();
    for ( ; itrLHS != endLHS; ++itrLHS )
    {
        DataType::ConstIterator itrRHS = rhsData.Find( *itrLHS );
        if( itrRHS == endRHS )
        {
            return false;
        }
    }

    return true;
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Serialize( ArchiveBinary& archive )
{
    Serialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Deserialize( ArchiveBinary& archive )
{
    Deserialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Serialize( ArchiveXML& archive )
{
    Serialize<ArchiveXML>( archive );
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Deserialize( ArchiveXML& archive )
{
    Deserialize<ArchiveXML>( archive );
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
tostream& SimpleSetData< KeyT, EqualKeyT, AllocatorT >::operator>>( tostream& stream ) const
{
    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        if ( itr != m_Data->Begin() )
        {
            stream << s_ContainerItemDelimiter;
        }

        stream << *itr;
    }

    return stream;
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT >
tistream& SimpleSetData< KeyT, EqualKeyT, AllocatorT >::operator<<( tistream& stream )
{
    m_Data->Clear();

    String str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.Reserve( static_cast< size_t >( size ) );
    str.Resize( static_cast< size_t >( size ) );
    stream.read( &str[ 0 ], size );

    Tokenize< KeyT, EqualKeyT, AllocatorT >( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}  

template< typename KeyT, typename EqualKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Serialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    components.Reserve( m_Data->GetSize() );

    {
        DataType::ConstIterator itr = m_Data->Begin();
        DataType::ConstIterator end = m_Data->End();
        for ( ; itr != end; ++itr )
        {
            ObjectPtr dataElem = Registry::GetInstance()->CreateInstance( Reflect::GetDataClass< KeyT >() );

            // downcast to data type
            Data* dataSer = AssertCast< Data >( dataElem );

            // connect to our map data memory address
            dataSer->ConnectData( const_cast< KeyT* >( &( *itr ) ) );

            // serialize to the archive stream
            HELIUM_VERIFY( components.New( dataSer ) );
        }
    }

    archive.SerializeArray( components );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    for ( ; itr != end; ++itr )
    {
        Data* ser = AssertCast< Data >( *itr );
        ser->Disconnect();

        // might be useful to cache the data object here
    }
}

template< typename KeyT, typename EqualKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleSetData< KeyT, EqualKeyT, AllocatorT >::Deserialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    archive.DeserializeArray( components );

    // if we are referring to a real field, clear its contents
    m_Data->Clear();
    m_Data->Reserve( components.GetSize() );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    for ( ; itr != end; ++itr )
    {
        Data* data = SafeCast< Data >( *itr );
        if ( !data )
        {
            throw LogisticException( TXT( "Set value type has changed, this is unpossible" ) );
        }

        KeyT k;
        Data::GetValue( data, k );
        m_Data->Insert( k );
    }
}


template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::ConnectData( void* data )
{
    m_Data.Connect( data );
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
size_t SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::GetSize() const
{
    return m_Data->GetSize();
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Clear()
{
    return m_Data->Clear();
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
const Class* SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::GetItemClass() const
{
    return Reflect::GetDataClass< KeyT >();
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::GetItems( DynamicArray< DataPtr >& items ) const
{
    items.Clear();
    items.Reserve( m_Data->GetSize() );

    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        HELIUM_VERIFY( items.New( Data::Bind( const_cast< KeyT& >( *itr ), m_Instance, m_Field ) ) );
    }
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::AddItem( Data* value )
{
    KeyT dataValue;
    Data::GetValue( value, dataValue );
    m_Data->Insert( dataValue );
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::RemoveItem( Data* value )
{
    KeyT dataValue;
    Data::GetValue( value, dataValue );
    m_Data->Remove( dataValue );
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
bool SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::ContainsItem( Data* value ) const
{
    KeyT dataValue;
    Data::GetValue( value, dataValue );
    return m_Data->Find( dataValue ) != m_Data->End();
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
bool SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Set( Data* src, uint32_t flags )
{
    const SortedSetDataT* rhs = SafeCast< SortedSetDataT >( src );
    if ( !rhs )
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
bool SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Equals( Object* object )
{
    const SortedSetDataT* rhs = SafeCast< SortedSetDataT >( object );
    if ( !rhs )
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Serialize( ArchiveBinary& archive )
{
    Serialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Deserialize( ArchiveBinary& archive )
{
    Deserialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Serialize( ArchiveXML& archive )
{
    Serialize<ArchiveXML>( archive );
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Deserialize( ArchiveXML& archive )
{
    Deserialize<ArchiveXML>( archive );
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
tostream& SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::operator>>( tostream& stream ) const
{
    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        if ( itr != m_Data->Begin() )
        {
            stream << s_ContainerItemDelimiter;
        }

        stream << *itr;
    }

    return stream;
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT >
tistream& SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::operator<<( tistream& stream )
{
    m_Data->Clear();

    String str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.Reserve( static_cast< size_t >( size ) );
    str.Resize( static_cast< size_t >( size ) );
    stream.read( &str[ 0 ], size );

    Tokenize< KeyT, CompareKeyT, AllocatorT >( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}  


template< typename KeyT, typename CompareKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Serialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    components.Reserve( m_Data->GetSize() );

    {
        DataType::ConstIterator itr = m_Data->Begin();
        DataType::ConstIterator end = m_Data->End();
        for ( ; itr != end; ++itr )
        {
            ObjectPtr dataElem = Registry::GetInstance()->CreateInstance( Reflect::GetDataClass< KeyT >() );

            // downcast to data type
            Data* dataSer = AssertCast< Data >( dataElem );

            // connect to our map data memory address
            dataSer->ConnectData( const_cast< KeyT* >( &( *itr ) ) );

            // serialize to the archive stream
            HELIUM_VERIFY( components.New( dataSer ) );
        }
    }

    archive.SerializeArray( components );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    for ( ; itr != end; ++itr )
    {
        Data* ser = AssertCast< Data >( *itr );
        ser->Disconnect();

        // might be useful to cache the data object here
    }
}

template< typename KeyT, typename CompareKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleSortedSetData< KeyT, CompareKeyT, AllocatorT >::Deserialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    archive.DeserializeArray( components );

    // if we are referring to a real field, clear its contents
    m_Data->Clear();
    m_Data->Reserve( components.GetSize() );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    for ( ; itr != end; ++itr )
    {
        Data* data = SafeCast< Data >( *itr );
        if ( !data )
        {
            throw LogisticException( TXT( "SortedSet value type has changed, this is unpossible" ) );
        }

        KeyT k;
        Data::GetValue( data, k );
        m_Data->Insert( k );
    }
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
size_t SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::GetSize() const
{
    return m_Data->GetSize();
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Clear()
{
    return m_Data->Clear();
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::ConnectData(void* data)
{
    m_Data.Connect( data );
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
const Class* SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::GetKeyClass() const
{
    return Reflect::GetDataClass< KeyT >();
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
const Class* SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::GetValueClass() const
{
    return Reflect::GetDataClass< ValueT >();
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::GetItems( A_ValueType& items )
{
    items.Clear();
    items.Reserve( m_Data->GetSize() );

    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        HELIUM_VERIFY( items.New(
            Data::Bind( const_cast< KeyT& >( itr->First() ), m_Instance, m_Field ),
            Data::Bind( const_cast< ValueT& >( itr->Second() ), m_Instance, m_Field ) ) );
    }
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
DataPtr SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::GetItem( Data* key )
{
    KeyT keyValue;
    Data::GetValue( key, keyValue );

    DataType::ConstIterator found = m_Data->Find( keyValue );
    if ( found != m_Data->End() )
    {
        return Data::Bind( const_cast< ValueT& >( found->Second() ), m_Instance, m_Field );
    }

    return NULL;
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::SetItem( Data* key, Data* value )
{
    KeyT keyValue;
    Data::GetValue( key, keyValue );

    Data::GetValue( value, (*m_Data)[ keyValue ] );
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::RemoveItem( Data* key )
{
    KeyT keyValue;
    Data::GetValue( key, keyValue );

    m_Data->Remove( keyValue );
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
bool SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Set( Data* src, uint32_t flags )
{
    const MapDataT* rhs = SafeCast< MapDataT >( src );
    if ( !rhs )
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
bool SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Equals( Object* object )
{
    const MapDataT* rhs = SafeCast< MapDataT >( object );
    if ( !rhs )
    {
        return false;
    }

    if ( m_Data->GetSize() != rhs->m_Data->GetSize() )
    {
        return false;
    }

    const DataType& rhsData = *m_Data;

    DataType::ConstIterator itrLHS = m_Data->Begin();
    DataType::ConstIterator endLHS = m_Data->End();
    DataType::ConstIterator endRHS = rhsData.End();
    for ( ; itrLHS != endLHS; ++itrLHS )
    {
        DataType::ConstIterator itrRHS = rhsData.Find( itrLHS->First() );
        if( itrRHS == endRHS || itrLHS->Second() != itrRHS->Second() )
        {
            return false;
        }
    }

    return true;
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Serialize( ArchiveBinary& archive )
{
    Serialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Deserialize( ArchiveBinary& archive )
{
    Deserialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Serialize( ArchiveXML& archive )
{
    Serialize<ArchiveXML>( archive );
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Deserialize( ArchiveXML& archive )
{
    Deserialize<ArchiveXML>( archive );
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
tostream& SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::operator>>( tostream& stream ) const
{
    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        if ( itr != m_Data->Begin() )
        {
            stream << s_ContainerItemDelimiter;
        }

        stream << itr->First() << s_ContainerItemDelimiter << itr->Second();
    }

    return stream;
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT >
tistream& SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::operator<<( tistream& stream )
{
    m_Data->Clear();

    String str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.Reserve( static_cast< size_t >( size ) );
    str.Resize( static_cast< size_t >( size ) );
    stream.read( &str[ 0 ], size );

    Tokenize< KeyT, ValueT, EqualKeyT, AllocatorT >( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Serialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    components.Reserve( m_Data->GetSize() * 2 );

    {
        DataType::ConstIterator itr = m_Data->Begin();
        DataType::ConstIterator end = m_Data->End();
        for ( ; itr != end; ++itr )
        {
            ObjectPtr keyElem = Registry::GetInstance()->CreateInstance( Reflect::GetDataClass< KeyT >() );
            ObjectPtr dataElem = Registry::GetInstance()->CreateInstance( Reflect::GetDataClass< ValueT >() );

            Data* keySer = AssertCast< Data >( keyElem );
            Data* dataSer = AssertCast< Data >( dataElem );

            // connect to our map key memory address
            keySer->ConnectData( const_cast< KeyT* >( &itr->First() ) );

            // connect to our map data memory address
            dataSer->ConnectData( const_cast< ValueT* >( &itr->Second() ) );

            // serialize to the archive stream
            HELIUM_VERIFY( components.New( keySer ) );
            HELIUM_VERIFY( components.New( dataSer ) );
        }
    }

    archive.SerializeArray( components );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    for ( ; itr != end; ++itr )
    {
        Data* ser = AssertCast< Data >( *itr );
        HELIUM_ASSERT( ser );
        ser->Disconnect();

        // might be useful to cache the data object here
    }
}

template< typename KeyT, typename ValueT, typename EqualKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT >::Deserialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    archive.DeserializeArray( components, ArchiveFlags::Sparse );

    size_t componentCount = components.GetSize();
    if ( componentCount % 2 != 0 )
    {
        throw Reflect::DataFormatException( TXT( "Unmatched map objects" ) );
    }

    // if we are referring to a real field, clear its contents
    m_Data->Clear();
    m_Data->Reserve( componentCount / 2 );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    while( itr != end )
    {
        Data* key = SafeCast< Data >( *itr );
        ++itr;
        Data* value = SafeCast< Data >( *itr );
        ++itr;

        if ( key && value )
        {
            KeyT k;
            Data::GetValue( key, k );
            Data::GetValue( value, (*m_Data)[ k ] );
        }
    }
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
size_t SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::GetSize() const
{
    return m_Data->GetSize();
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Clear()
{
    return m_Data->Clear();
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::ConnectData(void* data)
{
    m_Data.Connect( data );
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
const Class* SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::GetKeyClass() const
{
    return Reflect::GetDataClass< KeyT >();
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
const Class* SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::GetValueClass() const
{
    return Reflect::GetDataClass< ValueT >();
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::GetItems( A_ValueType& items )
{
    items.Clear();
    items.Reserve( m_Data->GetSize() );

    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        HELIUM_VERIFY( items.New( Data::Bind( const_cast< KeyT& >( itr->First() ), m_Instance, m_Field ), Data::Bind( const_cast< ValueT& >( itr->Second() ), m_Instance, m_Field ) ) );
    }
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
DataPtr SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::GetItem( Data* key )
{
    KeyT keyValue;
    Data::GetValue( key, keyValue );

    DataType::ConstIterator found = m_Data->Find( keyValue );
    if ( found != m_Data->End() )
    {
        return Data::Bind( const_cast< ValueT& >( found->Second() ), m_Instance, m_Field );
    }

    return NULL;
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::SetItem( Data* key, Data* value )
{
    KeyT keyValue;
    Data::GetValue( key, keyValue );

    Data::GetValue( value, (*m_Data)[ keyValue ] );
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::RemoveItem( Data* key )
{
    KeyT keyValue;
    Data::GetValue( key, keyValue );

    m_Data->Remove( keyValue );
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
bool SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Set( Data* src, uint32_t flags )
{
    const SortedMapDataT* rhs = SafeCast< SortedMapDataT >( src );
    if ( !rhs )
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
bool SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Equals( Object* object )
{
    const SortedMapDataT* rhs = SafeCast< SortedMapDataT >( object );
    if ( !rhs )
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Serialize( ArchiveBinary& archive )
{
    Serialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Deserialize( ArchiveBinary& archive )
{
    Deserialize<ArchiveBinary>( archive );
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Serialize( ArchiveXML& archive )
{
    Serialize<ArchiveXML>( archive );
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Deserialize( ArchiveXML& archive )
{
    Deserialize<ArchiveXML>( archive );
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
tostream& SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::operator>>( tostream& stream ) const
{
    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        if ( itr != m_Data->Begin() )
        {
            stream << s_ContainerItemDelimiter;
        }

        stream << itr->First() << s_ContainerItemDelimiter << itr->Second();
    }

    return stream;
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT >
tistream& SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::operator<<( tistream& stream )
{
    m_Data->Clear();

    String str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.Reserve( static_cast< size_t >( size ) );
    str.Resize( static_cast< size_t >( size ) );
    stream.read( &str[ 0 ], size );

    Tokenize< KeyT, ValueT, CompareKeyT, AllocatorT >( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}  

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Serialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    components.Reserve( m_Data->GetSize() * 2 );

    {
        DataType::ConstIterator itr = m_Data->Begin();
        DataType::ConstIterator end = m_Data->End();
        for ( ; itr != end; ++itr )
        {
            ObjectPtr keyElem = Registry::GetInstance()->CreateInstance( Reflect::GetDataClass< KeyT >() );
            ObjectPtr dataElem = Registry::GetInstance()->CreateInstance( Reflect::GetDataClass< ValueT >() );

            Data* keySer = AssertCast< Data >( keyElem );
            Data* dataSer = AssertCast< Data >( dataElem );

            // connect to our map key memory address
            keySer->ConnectData( const_cast< KeyT* >( &itr->First() ) );

            // connect to our map data memory address
            dataSer->ConnectData( const_cast< ValueT* >( &itr->Second() ) );

            // serialize to the archive stream
            HELIUM_VERIFY( components.New( keySer ) );
            HELIUM_VERIFY( components.New( dataSer ) );
        }
    }

    archive.SerializeArray( components );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    for ( ; itr != end; ++itr )
    {
        Data* ser = AssertCast< Data >( *itr );
        HELIUM_ASSERT( ser );
        ser->Disconnect();

        // might be useful to cache the data object here
    }
}

template< typename KeyT, typename ValueT, typename CompareKeyT, typename AllocatorT > template< class ArchiveT >
void SimpleSortedMapData< KeyT, ValueT, CompareKeyT, AllocatorT >::Deserialize( ArchiveT& archive )
{
    DynamicArray< ObjectPtr > components;
    archive.DeserializeArray( components, ArchiveFlags::Sparse );

    size_t componentCount = components.GetSize();
    if ( componentCount % 2 != 0 )
    {
        throw Reflect::DataFormatException( TXT( "Unmatched map objects" ) );
    }

    // if we are referring to a real field, clear its contents
    m_Data->Clear();
    m_Data->Reserve( componentCount / 2 );

    DynamicArray< ObjectPtr >::Iterator itr = components.Begin();
    DynamicArray< ObjectPtr >::Iterator end = components.End();
    while( itr != end )
    {
        Data* key = SafeCast< Data >( *itr );
        ++itr;
        Data* value = SafeCast< Data >( *itr );
        ++itr;

        if ( key && value )
        {
            KeyT k;
            Data::GetValue( key, k );
            Data::GetValue( value, (*m_Data)[ k ] );
        }
    }
}

#endif