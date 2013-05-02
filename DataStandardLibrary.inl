template <class T>
Helium::Reflect::SimpleStlVectorData<T>::SimpleStlVectorData()
    : SequenceData(sizeof(std::vector<T>)),
        m_InternalData(AllocateData<T>())
{
                
}

template <class T>
Helium::Reflect::SimpleStlVectorData<T>::~SimpleStlVectorData()
{
    delete m_InternalData;
}
        
template <class T>
void SimpleStlVectorData<T>::Construct( DataPointer pointer )
{
    DefaultConstruct< std::vector<T> >(pointer);
}
        
template <class T>
void SimpleStlVectorData<T>::Destruct( DataPointer pointer )
{
    DefaultDestruct< std::vector<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags /*= 0 */ )
{
    if (flags & CopyFlags::Shallow)
    {
        DefaultCopy< std::vector<T> >(src, dest, flags);
        return;
    }

    std::vector<T> &v_src = src.As< std::vector<T> >();
    std::vector<T> &v_dest = dest.As< std::vector<T> >();

    v_dest.resize(v_src.size());

    std::vector<T>::iterator iter_src = v_src.begin();
    std::vector<T>::iterator iter_dest = v_dest.begin();

    for (; iter_src != v_src.end(); ++iter_src, ++iter_dest)
    {
        DataPointer dp_src(&*iter_src, src.m_Field, src.m_Object);
        DataPointer dp_dest(&*iter_dest, dest.m_Field, dest.m_Object);
        m_InternalData->Copy(dp_src, dp_dest, flags);
    }
}

template <class T>
bool Helium::Reflect::SimpleStlVectorData<T>::Equals( DataPointer a, DataPointer b )
{
    std::vector<T> &v_a = a.As< std::vector<T> >();
    std::vector<T> &v_b = b.As< std::vector<T> >();

    if (v_a.size() != v_b.size())
    {
        return false;
    }
            
    std::vector<T>::iterator iter_a = v_a.begin();
    std::vector<T>::iterator iter_b = v_b.begin();
    for (; iter_a != v_a.end(); ++iter_a, ++iter_b)
    {
        DataPointer dp_a(&*iter_a, a.m_Field, b.m_Object);
        DataPointer dp_b(&*iter_b, b.m_Field, b.m_Object);

        if (!m_InternalData->Equals(dp_a, dp_b))
        {
            return false;
        }
    }

    return true;
}
        
template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::Accept( DataPointer p, Visitor& visitor )
{
    std::vector<T> &v = p.As< std::vector<T> >();
            
    for (std::vector<T>::iterator iter = v.begin();
        iter != v.end(); ++iter)
    {  
        m_InternalData->Accept(DataPointer(&*iter, p.m_Field, p.m_Object), visitor);
    }
}
        
template <class T>
size_t Helium::Reflect::SimpleStlVectorData<T>::GetLength( DataPointer container ) const
{
    std::vector<T> &v = container.As< std::vector<T> >();
    return v.size();
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::Clear( DataPointer container )
{
    std::vector<T> &v = container.As< std::vector<T> >();
    v.clear();
}

template <class T>
Data* Helium::Reflect::SimpleStlVectorData<T>::GetItemData() const
{
    return m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::GetItems( DataPointer sequence, DynamicArray< DataPointer >& items ) const
{
    std::vector<T> &v = sequence.As< std::vector<T> >();
            
    items.Resize(v.size());
            
    int i = 0;
    std::vector<T>::iterator iter = v.begin();
    for (; iter != v.end(); ++iter)
    {
        items[i++] = DataPointer(&*iter, sequence.m_Field, sequence.m_Object);
    }
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::SetLength( DataPointer sequence, size_t length )
{
    std::vector<T> &v = sequence.As< std::vector<T> >();
    v.resize(length);
}
        
template <class T>
DataPointer Helium::Reflect::SimpleStlVectorData<T>::GetItem( DataPointer sequence, size_t at )
{
    std::vector<T> &v = sequence.As< std::vector<T> >();
    return DataPointer(&v[at], sequence.m_Field, sequence.m_Object);
}
        
template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::SetItem( DataPointer sequence, size_t at, DataPointer value )
{
    std::vector<T> &v = sequence.As< std::vector<T> >();
    m_InternalData->Copy(value, DataPointer(&v[at], sequence.m_Field, sequence.m_Object));
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::Insert( DataPointer sequence, size_t at, DataPointer value )
{
    std::vector<T> &v = sequence.As< std::vector<T> >();
    v.insert(v.begin() + at, value.As<T>());
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::Remove( DataPointer sequence, size_t at )
{
    std::vector<T> &v = sequence.As< std::vector<T> >();
    v.erase(v.begin() + at);
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::MoveUp( DataPointer sequence, Set< size_t >& items )
{
    for (Set<size_t>::Iterator iter = items.Begin(); 
        iter != items.End(); ++iter)
    {
        size_t index = *iter;

        if (index <= 0)
        {
            continue;
        }

        SwapInternalValues(sequence, index, index - 1);
    }
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::MoveDown( DataPointer sequence, Set< size_t >& items )
{
    std::vector<T> &v = sequence.As< std::vector<T> >();
            
    for (Set<size_t>::Iterator iter = items.End() - 1; 
        iter >= items.Begin(); --iter)
    {
        size_t index = *iter;

        if (index >= v.size() - 1)
        {
            continue;
        }

        SwapInternalValues(sequence, index, index + 1);
    }
}
        
template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::SwapInternalValues( DataPointer sequence, size_t a, size_t b )
{
    std::vector<T> &v = sequence.As< std::vector<T> >();

    HELIUM_ASSERT(a >= 0);
    HELIUM_ASSERT(b >= 0);

    HELIUM_ASSERT(a < v.size());
    HELIUM_ASSERT(b < v.size());

    HELIUM_ASSERT(a != b);

    T temp;
            
    std::vector<T>::iterator iter_a = v.begin() + a;
    std::vector<T>::iterator iter_b = v.begin() + b;

    DataPointer dp_a(&*iter_a, sequence.m_Field, sequence.m_Object);
    DataPointer dp_b(&*iter_b, sequence.m_Field, sequence.m_Object);
    DataPointer dp_temp(&temp, sequence.m_Field, sequence.m_Object);

    m_InternalData->Copy(dp_a, dp_temp);
    m_InternalData->Copy(dp_b, dp_a);
    m_InternalData->Copy(dp_temp, dp_b, CopyFlags::Notify);
}

//////////////////////////////////////////////////////////////////////////
        
        
template <class T>
Helium::Reflect::SimpleStlSetData<T>::SimpleStlSetData()
    : SetData(sizeof(std::set<T>)),
        m_InternalData(AllocateData<T>())
{

}

template <class T>
Helium::Reflect::SimpleStlSetData<T>::~SimpleStlSetData()
{
    delete m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::Construct( DataPointer pointer )
{
    DefaultConstruct< std::set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::Destruct( DataPointer pointer )
{
    DefaultDestruct< std::set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags /*= 0 */ )
{
    if (flags & CopyFlags::Shallow)
    {
        DefaultCopy< std::set< T > >(src, dest, flags);
        return;
    }

    std::set<T> &s_src = src.As< std::set<T> >();
    std::set<T> &s_dest = dest.As< std::set<T> >();

    s_dest.clear();

    for (std::set<T>::iterator iter_src = s_src.begin();
        iter_src != s_src.end(); ++iter_src)
    {
        // Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
        DataPointer dp_src(const_cast<T *>(&*iter_src), src.m_Field, src.m_Object);
        T temp;
        DataPointer dp_dest(&temp, dest.m_Field, dest.m_Object);
        m_InternalData->Copy(dp_src, dp_dest, flags);
        s_dest.insert(temp);
    }
}

template <class T>
bool Helium::Reflect::SimpleStlSetData<T>::Equals( DataPointer a, DataPointer b )
{
    std::set<T> &s_a = a.As< std::set<T> >();
    std::set<T> &s_b = b.As< std::set<T> >();

    if (s_a.size() != s_b.size())
    {
        return false;
    }

    std::set<T>::iterator iter_a = s_a.begin();
    std::set<T>::iterator iter_b = s_b.begin();

    for (; iter_a != s_a.end(); 
        ++iter_a, iter_b)
    {
        // Should be safe as long as equality comparison doesn't break const-ness (shouldn't!)
        DataPointer dp_a(const_cast<T *>(&*iter_a), a.m_Field, b.m_Object);
        DataPointer dp_b(const_cast<T *>(&*iter_b), b.m_Field, b.m_Object);

        if (!m_InternalData->Equals(dp_a, dp_b))
        {
            return false;
        }
    }

    return true;
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::Accept( DataPointer p, Visitor& visitor )
{
    std::set<T> &s = p.As< std::set<T> >();
            
    for (std::set<T>::iterator iter = s.begin();
        iter != s.end(); ++iter)
    {
        // This const cast is downright dangerous.. visitors can definitely change values
        DataPointer dp(const_cast<T *>(&*iter), p.m_Field, p.m_Object);
        m_InternalData->Accept(dp, visitor);
    }
}

template <class T>
size_t Helium::Reflect::SimpleStlSetData<T>::GetLength( DataPointer container ) const
{
    std::set<T> &s = container.As< std::set<T> >();
    return s.size();
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::Clear( DataPointer container )
{
    std::set<T> &s = container.As< std::set<T> >();
    s.clear();
}

template <class T>
Data* Helium::Reflect::SimpleStlSetData<T>::GetItemData() const
{
    return m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::GetItems( DataPointer set, DynamicArray< DataPointer >& items ) const
{
    std::set<T> &v = set.As< std::set<T> >();
    items.Resize(v.size());
            
    std::set<T>::iterator iter = v.begin();
    int i = 0;
    for (; iter != v.end(); ++iter)
    {
        // This is dangerous.. callers could modify values passed out
        DataPointer dp(const_cast<T *>(&*iter), set.m_Field, set.m_Object);
        items[i++] = dp;
    }
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::InsertItem( DataPointer set, DataPointer item )
{
    // Not crazy about this insert going through the copy ctor instead of our copy implementation for the data type
    std::set<T> &v = set.As< std::set<T> >();
    v.insert(std::set<T>::value_type(item.As<T>()));
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::RemoveItem( DataPointer set, DataPointer item )
{
    // Not crazy about this insert going through the c++ equal operator instead of the data type
    std::set<T> &v = set.As< std::set<T> >();
    v.erase(std::set<T>::value_type(item.As<T>()));
}

template <class T>
bool Helium::Reflect::SimpleStlSetData<T>::ContainsItem( DataPointer set, DataPointer item ) const
{
    std::set<T> &s = set.As< std::set<T> >();

    // This is an awful linear search instead of a log(n) search so that we can defer to the internal data class
    for (std::set<T>::iterator iter = s.begin(); iter != s.end(); ++iter)
    {
        // Should be safe since we are checking equality
        DataPointer a(const_cast<T *>(&*iter), set.m_Field, set.m_Object);

        if (!m_InternalData->Equals(a, item))
        {
            return false;
        }
    }

    return true;
}


#ifdef REFLECT_REFACTOR

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
            throw Reflect::Exception( TXT( "Set value type has changed, this is unpossible" ) );
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
        throw Reflect::Exception( TXT( "Unmatched map objects" ) );
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