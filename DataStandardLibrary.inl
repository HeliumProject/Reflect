#ifdef REFLECT_REFACTOR

template < class T >
void SimpleStlVectorData<T>::ConnectData(void* data)
{
    m_Data.Connect( data );
}

template < class T >
size_t SimpleStlVectorData<T>::GetSize() const
{ 
    return m_Data->size(); 
}

template < class T >
void SimpleStlVectorData<T>::SetSize(size_t size)
{
    return m_Data->resize(size);
}

template < class T >
void SimpleStlVectorData<T>::Clear()
{
    return m_Data->clear();
}

template < class T >
const Class* SimpleStlVectorData<T>::GetItemClass() const
{
    return Reflect::GetDataClass<T>();
}

template < class T >
DataPtr SimpleStlVectorData<T>::GetItem(size_t at)
{
    return Data::Bind(m_Data->at(at), m_Instance, m_Field);
}

template < class T >
void SimpleStlVectorData<T>::SetItem(size_t at, Data* value)
{
    Data::GetValue(value, m_Data->at(at));
}

template < class T >
void SimpleStlVectorData<T>::Insert( size_t at, Data* value )
{
    T temp;
    Data::GetValue( value, temp );
    m_Data->insert( m_Data->begin() + at, temp );
}

template < class T >
void SimpleStlVectorData<T>::Remove( size_t at )
{
    m_Data->erase( m_Data->begin() + at );
}

template < class T >
void SimpleStlVectorData<T>::MoveUp( std::set< size_t >& selectedIndices )
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
        
        T temp = m_Data->at( (*itr) - 1 );
        m_Data->at( (*itr) - 1 ) = m_Data->at( (*itr) );
        m_Data->at( (*itr) ) = temp;

        newSelectedIndices.insert( *itr - 1 );
    }

    selectedIndices = newSelectedIndices;
}

template < class T >
void SimpleStlVectorData<T>::MoveDown( std::set< size_t >& selectedIndices )
{
    std::set< size_t > newSelectedIndices;

    std::set< size_t >::const_reverse_iterator itr = selectedIndices.rbegin();
    std::set< size_t >::const_reverse_iterator end = selectedIndices.rend();

    for( ; itr != end; ++itr )
    {
        if ( ( (*itr) == m_Data->size() - 1 ) || ( newSelectedIndices.find( (*itr) + 1 ) != newSelectedIndices.end() ) )
        {
            newSelectedIndices.insert( *itr );
            continue;
        }
        
        T temp = m_Data->at( (*itr) + 1 );
        m_Data->at( (*itr) + 1 ) = m_Data->at( (*itr) );
        m_Data->at( (*itr) ) = temp;

        newSelectedIndices.insert( *itr + 1 );
    }

    selectedIndices = newSelectedIndices;
}

template < class T >
bool SimpleStlVectorData<T>::Set(Data* src, uint32_t flags)
{
    const SimpleStlVectorData<T>* rhs = SafeCast<SimpleStlVectorData<T>>(src);
    if (!rhs)
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template < class T >
bool SimpleStlVectorData<T>::Equals(Object* object)
{
    const SimpleStlVectorData<T>* rhs = SafeCast< SimpleStlVectorData<T> >(object);
    if (!rhs)
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

//
// Stl specializes std::vector<bool> using bitfields...
//  in general this isn't very efficient, but isn't used often
//

template < class T >
void WriteVector( const std::vector< T >& v, Reflect::CharStream& stream )
{
    int32_t count = (int32_t)v.size();
    stream.Write(&count); 

    if (count > 0)
    {
        stream.WriteBuffer( (const void*)&(v.front()), sizeof(T) * count); 
    }

}

template <>
void WriteVector( const std::vector< bool >& v, Reflect::CharStream& stream )
{
    int32_t count = (int32_t)v.size();
    stream.Write(&count); 

    for ( std::vector< bool >::const_iterator itr = v.begin(), end = v.end(); itr != end; ++itr )
    {
        bool value = *itr;
        stream.Write( &value ); 
    }

}

template < class T >
void ReadVector( std::vector< T >& v, Reflect::CharStream& stream )
{
    int32_t count = 0;
    stream.Read(&count); 
    v.resize(count);

    if(count > 0)
    {
        stream.ReadBuffer( (void*)&(v.front()), sizeof(T) * count );
    }
}

template <>
void ReadVector( std::vector< bool >& v, Reflect::CharStream& stream )
{
    int32_t count = 0;
    stream.Read(&count); 
    v.reserve(count);

    while( --count > 0 )
    {
        bool value;
        stream.Read( &value );
        v.push_back( value );
    }
}

template < class T >
void SimpleStlVectorData<T>::Serialize(ArchiveBinary& archive)
{
    WriteVector( *m_Data, archive.GetStream() );
}

template < class T >
void SimpleStlVectorData<T>::Deserialize(ArchiveBinary& archive)
{
    // if we are referring to a real field, clear its contents
    m_Data->clear();
    ReadVector( *m_Data, archive.GetStream() );
}

template < class T >
void SimpleStlVectorData<T>::Serialize(ArchiveXML& archive)
{
    archive.GetIndent().Push();

    // foreach datum
    for (size_t i=0; i<m_Data->size(); i++)
    {
        // indent
        archive.GetIndent().Get(archive.GetStream());

        // write
        archive.GetStream() << m_Data->at( i );

        // newline
        archive.GetStream() << "\n";
    }

    archive.GetIndent().Pop();
}

template < class T >
void SimpleStlVectorData<T>::Deserialize(ArchiveXML& archive)
{
    // if we are referring to a real field, clear its contents
    m_Data->clear();

    T value;
    archive.GetStream().SkipWhitespace(); 

    while (!archive.GetStream().Done())
    {
        // read data
        archive.GetStream() >> value;

        // copy onto vector
        m_Data->push_back(value);

        // read to next non-whitespace char
        archive.GetStream().SkipWhitespace(); 
    }
}

template < class T >
tostream& SimpleStlVectorData<T>::operator>>(tostream& stream) const
{
    DataType::const_iterator itr = m_Data->begin();
    DataType::const_iterator end = m_Data->end();
    for ( ; itr != end; ++itr )
    {
        if ( itr != m_Data->begin() )
        {
            stream << s_ContainerItemDelimiter;
        }

        stream << *itr;
    }

    return stream;
}

template < class T >
tistream& SimpleStlVectorData<T>::operator<<(tistream& stream)
{
    m_Data->clear();

    tstring str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.resize( (size_t) size );
    stream.read( const_cast< tchar_t* >( str.c_str() ), size );

    Tokenize<T, T>( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}

//
// Specializations
//

template <>
void StlStringStlVectorData::Serialize(ArchiveBinary& archive)
{
    uint32_t count = (uint32_t)m_Data->size();
    archive.GetStream().Write( &count ); 

    for (size_t i=0; i<m_Data->size(); i++)
    {
        archive.GetStream().WriteString( m_Data->at( i ) ); 
    }
}

template <>
void StlStringStlVectorData::Deserialize(ArchiveBinary& archive)
{
    m_Data->clear();

    uint32_t count = (uint32_t)m_Data->size();
    archive.GetStream().Read( &count ); 

    m_Data->resize(count);
    for ( uint32_t i=0; i<count; i++ )
    {
        archive.GetStream().ReadString( m_Data->at( i ) ); 
    }
}

template <>
void StlStringStlVectorData::Serialize(ArchiveXML& archive)
{
    archive.GetIndent().Push();
    archive.GetIndent().Get(archive.GetStream());

    // start our CDATA section, this prevents XML from parsing its escapes in this cdata section
    archive.GetStream() << TXT("<![CDATA[\n");

    for (size_t i=0; i<m_Data->size(); i++)
    {
        archive.GetIndent().Get(archive.GetStream());

        // output the escape-code free character sequence between double qutoes
        archive.GetStream() << TXT('\"') << m_Data->at( i ) << TXT('\"') << s_ContainerItemDelimiter;
    }

    // end our CDATA escape section
    archive.GetIndent().Get(archive.GetStream());
    archive.GetStream() << TXT("]]>\n");

    archive.GetIndent().Pop();
}

template <>
void StlStringStlVectorData::Deserialize(ArchiveXML& archive)
{
    m_Data->clear();

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
            if (start == end-1)
            {
                m_Data->push_back(tstring ());
            }
            // else we have some non-null string data
            else
            {
                m_Data->push_back(value.substr(start + 1, end - start - 1));
            }
        }
        else
        {
            start = value.find_first_not_of( TXT( " \t\n" ) );

            if (start != std::string::npos)
                m_Data->push_back(value.substr(start));
        }

        archive.GetStream().SkipWhitespace(); 
    }
}

#ifdef HELIUM_WCHAR_T

//
// When --wchar_t is active the XML streams are made of wchar_t, and C++ stdlib won't do the conversion for uint8_t/int8_t
//  So we explicitly specialize some functions to to the conversion via a uint16_t/int16_t
//

template <>
tistream& SimpleStlVectorData<uint8_t>::operator<<(tistream& stream)
{
    m_Data->clear();

    tstring str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.resize( (size_t) size );
    stream.read(const_cast< tchar_t* >( str.c_str() ), size );

    Tokenize<uint8_t, uint16_t>( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}

template <>
tistream& SimpleStlVectorData<int8_t>::operator<<(tistream& stream)
{
    m_Data->clear();

    tstring str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.resize( (size_t) size );
    stream.read(const_cast< tchar_t* >( str.c_str() ), size );

    Tokenize<int8_t, int16_t>( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}

#endif // HELIUM_WCHAR_T

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::ConnectData(void* data)
{
    m_Data.Connect( data );
}

template < class DataT, class DataClassT >
size_t SimpleStlSetData<DataT, DataClassT>::GetSize() const
{
    return m_Data->size();
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::Clear()
{
    return m_Data->clear();
}

template < class DataT, class DataClassT >
const Class* SimpleStlSetData<DataT, DataClassT>::GetItemClass() const
{
    return Reflect::GetDataClass<DataT>();
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::GetItems(std::vector< DataPtr >& items) const
{
    items.resize( m_Data->size() );
    DataType::const_iterator itr = m_Data->begin();
    DataType::const_iterator end = m_Data->end();
    for ( size_t index=0; itr != end; ++itr, ++index )
    {
        items[index] = Data::Bind( const_cast< DataT& >( *itr ), m_Instance, m_Field );
    }
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::AddItem(Data* value)
{
    DataT dataValue;
    Data::GetValue(value, dataValue);
    m_Data->insert( dataValue );
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::RemoveItem(Data* value)
{
    DataT dataValue;
    Data::GetValue(value, dataValue);
    m_Data->erase( dataValue );
}

template < class DataT, class DataClassT >
bool SimpleStlSetData<DataT, DataClassT>::ContainsItem(Data* value) const
{
    DataT dataValue;
    Data::GetValue(value, dataValue);
    return m_Data->find( dataValue ) != m_Data->end();
}

template < class DataT, class DataClassT >
bool SimpleStlSetData<DataT, DataClassT>::Set(Data* src, uint32_t flags)
{
    const StlSetDataT* rhs = SafeCast<StlSetDataT>(src);
    if (!rhs)
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template < class DataT, class DataClassT >
bool SimpleStlSetData<DataT, DataClassT>::Equals(Object* object)
{
    const StlSetDataT* rhs = SafeCast<StlSetDataT>(object);
    if (!rhs)
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::Serialize( ArchiveBinary& archive )
{
    Serialize<ArchiveBinary>( archive );
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::Deserialize( ArchiveBinary& archive )
{
    Deserialize<ArchiveBinary>( archive );
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::Serialize( ArchiveXML& archive )
{
    Serialize<ArchiveXML>( archive );
}

template < class DataT, class DataClassT >
void SimpleStlSetData<DataT, DataClassT>::Deserialize( ArchiveXML& archive )
{
    Deserialize<ArchiveXML>( archive );
}

template < class DataT, class DataClassT >
tostream& SimpleStlSetData<DataT, DataClassT>::operator>>(tostream& stream) const
{
    DataType::const_iterator itr = m_Data->begin();
    DataType::const_iterator end = m_Data->end();
    for ( ; itr != end; ++itr )
    {
        if ( itr != m_Data->begin() )
        {
            stream << s_ContainerItemDelimiter;
        }

        stream << *itr;
    }

    return stream;
}

template < class DataT, class DataClassT >
tistream& SimpleStlSetData<DataT, DataClassT>::operator<<(tistream& stream)
{
    m_Data->clear();

    tstring str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.resize( (size_t) size);
    stream.read( const_cast< tchar_t* >( str.c_str() ), size );

    Tokenize< DataT >( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}

template < class DataT, class DataClassT > template< class ArchiveT >
void SimpleStlSetData<DataT, DataClassT>::Serialize( ArchiveT& archive )
{
    int i = 0;
    std::vector< ObjectPtr > components;
    components.resize( m_Data->size() );

    {
        DataType::const_iterator itr = m_Data->begin();
        DataType::const_iterator end = m_Data->end();
        for ( ; itr != end; ++itr )
        {
            ObjectPtr dataElem = Registry::GetInstance()->CreateInstance( Reflect::GetClass<DataClassT>() );

            // downcast to data type
            DataClassT* dataSer = AssertCast<DataClassT>(dataElem);

            // connect to our map data memory address
            dataSer->ConnectData(const_cast<DataT*>(&(*itr)));

            // serialize to the archive stream
            components[i++] = dataSer;
        }
    }

    archive.SerializeArray( components );

    std::vector< ObjectPtr >::iterator itr = components.begin();
    std::vector< ObjectPtr >::iterator end = components.end();
    for ( ; itr != end; ++itr )
    {
        Data* ser = AssertCast<Data>(*itr);
        ser->Disconnect();

        // might be useful to cache the data object here
    }
}

template < class DataT, class DataClassT > template < class ArchiveT >
void SimpleStlSetData<DataT, DataClassT>::Deserialize( ArchiveT& archive )
{
    std::vector< ObjectPtr > components;
    archive.DeserializeArray(components);

    // if we are referring to a real field, clear its contents
    m_Data->clear();

    std::vector< ObjectPtr >::iterator itr = components.begin();
    std::vector< ObjectPtr >::iterator end = components.end();
    for ( ; itr != end; ++itr )
    {
        DataClassT* data = SafeCast<DataClassT>(*itr);
        if (!data)
        {
            throw LogisticException( TXT( "Set value type has changed, this is unpossible" ) );
        }

        m_Data->insert( *data->m_Data );
    }
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
size_t SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::GetSize() const
{
    return m_Data->size();
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Clear()
{
    return m_Data->clear();
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::ConnectData(void* data)
{
    m_Data.Connect( data );
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
const Class* SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::GetKeyClass() const
{
    return Reflect::GetDataClass<KeyT>();
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
const Class* SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::GetValueClass() const
{
    return Reflect::GetDataClass<ValueT>();
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::GetItems(V_ValueType& items)
{
    items.resize(m_Data->size());
    DataType::const_iterator itr = m_Data->begin();
    DataType::const_iterator end = m_Data->end();
    for ( size_t index=0; itr != end; ++itr, ++index )
    {
        items[index].first = Data::Bind( const_cast< KeyT& >( itr->first ), m_Instance, m_Field );
        items[index].second = Data::Bind( const_cast< ValueT& >( itr->second ), m_Instance, m_Field );
    }
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
DataPtr SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::GetItem(Data* key)
{
    KeyT keyValue;
    Data::GetValue(key, keyValue);

    DataType::const_iterator found = m_Data->find( keyValue );
    if ( found != m_Data->end() )
    {
        return Data::Bind( const_cast< ValueT& >( found->second ), m_Instance, m_Field );
    }

    return NULL;
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::SetItem(Data* key, Data* value)
{
    KeyT keyValue;
    Data::GetValue(key, keyValue);

    ValueT valueValue;
    Data::GetValue(value, valueValue);

    (*m_Data)[keyValue] = valueValue;
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::RemoveItem(Data* key)
{
    KeyT keyValue;
    Data::GetValue(key, keyValue);

    m_Data->erase(keyValue);
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
bool SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Set(Data* src, uint32_t flags)
{
    const StlMapDataT* rhs = SafeCast<StlMapDataT>(src);
    if (!rhs)
    {
        return false;
    }

    *m_Data = *rhs->m_Data;

    return true;
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
bool SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Equals(Object* object)
{
    const StlMapDataT* rhs = SafeCast<StlMapDataT>(object);
    if (!rhs)
    {
        return false;
    }

    return *m_Data == *rhs->m_Data;
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Serialize( ArchiveBinary& archive )
{
    Serialize<ArchiveBinary>( archive );
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Deserialize( ArchiveBinary& archive )
{
    Deserialize<ArchiveBinary>( archive );
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Serialize( ArchiveXML& archive )
{
    Serialize<ArchiveXML>( archive );
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Deserialize( ArchiveXML& archive )
{
    Deserialize<ArchiveXML>( archive );
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
tostream& SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::operator>>(tostream& stream) const
{
    DataType::const_iterator itr = m_Data->begin();
    DataType::const_iterator end = m_Data->end();
    for ( ; itr != end; ++itr )
    {
        if ( itr != m_Data->begin() )
        {
            stream << s_ContainerItemDelimiter;
        }

        stream << itr->first << s_ContainerItemDelimiter << itr->second;
    }

    return stream;
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT >
tistream& SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::operator<<(tistream& stream)
{
    m_Data->clear();

    tstring str;
    std::streamsize size = stream.rdbuf()->in_avail();
    str.resize( (size_t) size);
    stream.read( const_cast< tchar_t* >( str.c_str() ), size );

    Tokenize< KeyT, ValueT >( str, *m_Data, s_ContainerItemDelimiter );

    return stream;
}  

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT > template< class ArchiveT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Serialize(ArchiveT& archive)
{
    int i = 0;
    std::vector< ObjectPtr > components;
    components.resize( m_Data->size() * 2 );

    {
        DataType::const_iterator itr = m_Data->begin();
        DataType::const_iterator end = m_Data->end();
        for ( ; itr != end; ++itr )
        {
            ObjectPtr keyElem = Registry::GetInstance()->CreateInstance( Reflect::GetClass<KeyClassT>() );
            ObjectPtr dataElem = Registry::GetInstance()->CreateInstance( Reflect::GetClass<ValueClassT>() );

            // downcast to data type
            KeyClassT* keySer = AssertCast<KeyClassT>(keyElem);
            ValueClassT* dataSer = AssertCast<ValueClassT>(dataElem);

            // connect to our map key memory address
            keySer->ConnectData(const_cast<KeyT*>(&(itr->first)));

            // connect to our map data memory address
            dataSer->ConnectData(const_cast<ValueT*>(&(itr->second)));

            // serialize to the archive stream
            components[i++] = keySer;
            components[i++] = dataSer;
        }
    }

    archive.SerializeArray( components );

    std::vector< ObjectPtr >::iterator itr = components.begin();
    std::vector< ObjectPtr >::iterator end = components.end();
    for ( ; itr != end; ++itr )
    {
        Data* ser = AssertCast<Data>(*itr);
        ser->Disconnect();

        // might be useful to cache the data object here
    }
}

template < class KeyT, class KeyClassT, class ValueT, class ValueClassT > template< class ArchiveT >
void SimpleStlMapData<KeyT, KeyClassT, ValueT, ValueClassT>::Deserialize(ArchiveT& archive)
{
    std::vector< ObjectPtr > components;
    archive.DeserializeArray(components, ArchiveFlags::Sparse);

    if (components.size() % 2 != 0)
    {
        throw Reflect::DataFormatException( TXT( "Unmatched map objects" ) );
    }

    // if we are referring to a real field, clear its contents
    m_Data->clear();

    std::vector< ObjectPtr >::iterator itr = components.begin();
    std::vector< ObjectPtr >::iterator end = components.end();
    for ( ; itr != end; ++itr )
    {
        KeyClassT* key = SafeCast<KeyClassT>( *itr );
        ValueClassT* value = SafeCast<ValueClassT>( *(++itr) );

        if (key && value)
        {
            (*m_Data)[ *key->m_Data ] = *value->m_Data;
        }
    }
}

#endif