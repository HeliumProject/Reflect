


template <class T>
Helium::Reflect::SimpleDynamicArrayData<T>::SimpleDynamicArrayData()
    : SequenceData(sizeof(DynamicArray<T>))
    , m_InternalData(AllocateData<T>())
{

}

template <class T>
Helium::Reflect::SimpleDynamicArrayData<T>::~SimpleDynamicArrayData()
{
	delete m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::Construct( DataPointer pointer )
{
	DefaultConstruct< std::vector<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::Destruct( DataPointer pointer )
{
	DefaultDestruct< DynamicArray<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags /*= 0 */ )
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< DynamicArray<T> >(src, dest, flags);
		return;
	}

	DynamicArray<T> &v_src = src.As< DynamicArray<T> >();
	DynamicArray<T> &v_dest = dest.As< DynamicArray<T> >();

	v_dest.Resize(v_src.GetSize());

	DynamicArray<T>::Iterator iter_src = v_src.Begin();
	DynamicArray<T>::Iterator iter_dest = v_dest.Begin();

	for (; iter_src != v_src.End(); ++iter_src, ++iter_dest)
	{
		DataPointer dp_src(&*iter_src, src.m_Field, src.m_Object);
		DataPointer dp_dest(&*iter_dest, dest.m_Field, dest.m_Object);
		m_InternalData->Copy(dp_src, dp_dest, flags);
	}
}

template <class T>
bool Helium::Reflect::SimpleDynamicArrayData<T>::Equals( DataPointer a, DataPointer b )
{
#if 0
	DynamicArray<T> &v_a = a.As< DynamicArray<T> >();
	DynamicArray<T> &v_b = b.As< DynamicArray<T> >();

	if (v_a.GetSize() != v_b.GetSize())
	{
		return false;
	}

	DynamicArray<T>::Iterator iter_a = v_a.begin();
	DynamicArray<T>::iterator iter_b = v_b.begin();
	for (; iter_a != v_a.End(); ++iter_a, ++iter_b)
	{
		DataPointer dp_a(&*iter_a, a.m_Field, b.m_Object);
		DataPointer dp_b(&*iter_b, b.m_Field, b.m_Object);

		if (!m_InternalData->Equals(dp_a, dp_b))
		{
			return false;
		}
	}

	return true;
#endif

    return DefaultEquals< DynamicArray<T> >(a, b);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::Accept( DataPointer p, Visitor& visitor )
{
	DynamicArray<T> &v = p.As< DynamicArray<T> >();

	for (DynamicArray<T>::Iterator iter = v.Begin();
		iter != v.End(); ++iter)
	{  
		m_InternalData->Accept(DataPointer(&*iter, p.m_Field, p.m_Object), visitor);
	}
}

template <class T>
size_t Helium::Reflect::SimpleDynamicArrayData<T>::GetLength( DataPointer container ) const
{
	DynamicArray<T> &v = container.As< DynamicArray<T> >();
	return v.GetSize();
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::Clear( DataPointer container )
{
	DynamicArray<T> &v = container.As< DynamicArray<T> >();
	v.Clear();
}

template <class T>
Helium::Reflect::Data* Helium::Reflect::SimpleDynamicArrayData<T>::GetItemData() const
{
	return m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::GetItems( DataPointer sequence, DynamicArray< DataPointer >& items ) const
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();

	items.Reserve(v.GetSize());

	for (DynamicArray<T>::Iterator iter = v.Begin(); iter != v.End(); ++iter)
	{
		items.Add(DataPointer(&*iter, sequence.m_Field, sequence.m_Object));
	}
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::SetLength( DataPointer sequence, size_t length )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	v.Resize(length);
}

template <class T>
Helium::Reflect::DataPointer Helium::Reflect::SimpleDynamicArrayData<T>::GetItem( DataPointer sequence, size_t at )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	return DataPointer(&v[at], sequence.m_Field, sequence.m_Object);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::SetItem( DataPointer sequence, size_t at, DataPointer value )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	m_InternalData->Copy(value, DataPointer(&v[at], sequence.m_Field, sequence.m_Object));
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::Insert( DataPointer sequence, size_t at, DataPointer value )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	v.Insert(at, value.As<T>());
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::Remove( DataPointer sequence, size_t at )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	v.Remove(at);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::MoveUp( DataPointer sequence, Set< size_t >& items )
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
void Helium::Reflect::SimpleDynamicArrayData<T>::MoveDown( DataPointer sequence, Set< size_t >& items )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();

	for (Set<size_t>::Iterator iter = items.End() - 1; 
		iter >= items.Begin(); --iter)
	{
		size_t index = *iter;

		if (index >= v.GetSize() - 1)
		{
			continue;
		}

		SwapInternalValues(sequence, index, index + 1);
	}
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayData<T>::SwapInternalValues( DataPointer sequence, size_t a, size_t b )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();

	HELIUM_ASSERT(a >= 0);
	HELIUM_ASSERT(b >= 0);

	HELIUM_ASSERT(a < v.GetSize());
	HELIUM_ASSERT(b < v.GetSize());

	HELIUM_ASSERT(a != b);

	T temp;

	DynamicArray<T>::Iterator iter_a = v.Begin() + a;
	DynamicArray<T>::Iterator iter_b = v.Begin() + b;

	DataPointer dp_a(&*iter_a, sequence.m_Field, sequence.m_Object);
	DataPointer dp_b(&*iter_b, sequence.m_Field, sequence.m_Object);
	DataPointer dp_temp(&temp, sequence.m_Field, sequence.m_Object);

	m_InternalData->Copy(dp_a, dp_temp);
	m_InternalData->Copy(dp_b, dp_a);
	m_InternalData->Copy(dp_temp, dp_b, CopyFlags::Notify);
}

//////////////////////////////////////////////////////////////////////////



template <class T>
Helium::Reflect::SimpleSetData<T>::SimpleSetData()
    : SetData(sizeof(Set<T>))
    , m_InternalData(AllocateData<T>())
{

}

template <class T>
Helium::Reflect::SimpleSetData<T>::~SimpleSetData()
{
	delete m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::Construct( DataPointer pointer )
{
	DefaultConstruct< Set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::Destruct( DataPointer pointer )
{
	DefaultDestruct< Set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::Copy( DataPointer src, DataPointer dest, uint32_t flags /*= 0 */ )
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< Set< T > >(src, dest, flags);
		return;
	}

	Set<T> &s_src = src.As< Set<T> >();
	Set<T> &s_dest = dest.As< Set<T> >();

	s_dest.Clear();

	for (Set<T>::Iterator iter_src = s_src.Begin();
		iter_src != s_src.End(); ++iter_src)
	{
		// Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
		DataPointer dp_src(const_cast<T *>(&*iter_src), src.m_Field, src.m_Object);
		T temp;
		DataPointer dp_dest(&temp, dest.m_Field, dest.m_Object);
		m_InternalData->Copy(dp_src, dp_dest, flags);
		s_dest.Insert(temp);
	}
}

template <class T>
bool Helium::Reflect::SimpleSetData<T>::Equals( DataPointer a, DataPointer b )
{
#if 0
	Set<T> &s_a = a.As< Set<T> >();
	Set<T> &s_b = b.As< Set<T> >();

	if (s_a.size() != s_b.size())
	{
		return false;
	}

	Set<T>::Iterator iter_a = s_a.Begin();
	Set<T>::Iterator iter_b = s_b.Begin();

	for (; iter_a != s_a.End(); 
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
#endif

    return DefaultEquals< Set<T> >(a, b);
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::Accept( DataPointer p, Visitor& visitor )
{
	Set<T> &s = p.As< Set<T> >();

	for (Set<T>::Iterator iter = s.Begin();
		iter != s.End(); ++iter)
	{
		// This const cast is downright dangerous.. visitors can definitely change values
		DataPointer dp(const_cast<T *>(&*iter), p.m_Field, p.m_Object);
		m_InternalData->Accept(dp, visitor);
	}
}

template <class T>
size_t Helium::Reflect::SimpleSetData<T>::GetLength( DataPointer container ) const
{
	Set<T> &s = container.As< Set<T> >();
	return s.GetSize();
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::Clear( DataPointer container )
{
	Set<T> &s = container.As< Set<T> >();
	s.Clear();
}

template <class T>
Helium::Reflect::Data* Helium::Reflect::SimpleSetData<T>::GetItemData() const
{
	return m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::GetItems( DataPointer set, DynamicArray< DataPointer >& items ) const
{
	Set<T> &v = set.As< Set<T> >();
	items.Reserve(v.GetSize());

	for (Set<T>::Iterator iter = v.Begin(); iter != v.End(); ++iter)
	{
		// This is dangerous.. callers could modify values passed out
		DataPointer dp(const_cast<T *>(&*iter), set.m_Field, set.m_Object);
		items.Add(dp);
	}
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::InsertItem( DataPointer set, DataPointer item )
{
	// Not crazy about this insert going through the copy ctor instead of our copy implementation for the data type
	Set<T> &v = set.As< Set<T> >();
	v.Insert(Set<T>::ValueType(item.As<T>()));
}

template <class T>
void Helium::Reflect::SimpleSetData<T>::RemoveItem( DataPointer set, DataPointer item )
{
	// Not crazy about this insert going through the c++ equal operator instead of the data type
	Set<T> &v = set.As< Set<T> >();
	v.Remove(Set<T>::ValueType(item.As<T>()));
}

template <class T>
bool Helium::Reflect::SimpleSetData<T>::ContainsItem( DataPointer set, DataPointer item ) const
{
	Set<T> &s = set.As< Set<T> >();

#if 0
	// This is an awful linear search instead of a log(n) search so that we can defer to the internal data class
	for (Set<T>::Iterator iter = s.Begin(); iter != s.End(); ++iter)
	{
		// Should be safe since we are checking equality
		DataPointer a(const_cast<T *>(&*iter), set.m_Field, set.m_Object);

		if (m_InternalData->Equals(a, item))
		{
			return true;
		}
	}
    
    return false;
#endif

    Set<T>::Iterator iter = s.Find(item.As<T>());
    return iter != s.End();
}

//////////////////////////////////////////////////////////////////////////


template <class KeyT, class ValueT>
Helium::Reflect::SimpleMapData<KeyT, ValueT>::SimpleMapData()
    : AssociationData(sizeof(Map<KeyT, ValueT>))
    , m_InternalDataKey(AllocateData<KeyT>())
    , m_InternalDataValue(AllocateData<ValueT>())
{

}

template <class KeyT, class ValueT>
Helium::Reflect::SimpleMapData<KeyT, ValueT>::~SimpleMapData()
{
    delete m_InternalDataKey;
    delete m_InternalDataValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::Construct( DataPointer pointer ) 
{
    DefaultConstruct< Map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::Destruct( DataPointer pointer ) 
{
    DefaultDestruct< Map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::Copy( DataPointer src, DataPointer dest, uint32_t flags /*= 0 */ ) 
{
    if (flags & CopyFlags::Shallow)
    {
        DefaultCopy< Map< KeyT, ValueT > >(src, dest, flags);
        return;
    }

    Map<KeyT, ValueT> &m_src = src.As< Map<KeyT, ValueT> >();
    Map<KeyT, ValueT> &m_dest = dest.As< Map<KeyT, ValueT> >();

    m_dest.Clear();

    for (Map<KeyT, ValueT>::Iterator iter_src = m_src.Begin();
        iter_src != m_src.End(); ++iter_src)
    {
        // Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
        DataPointer dp_src_key(const_cast<KeyT *>(&iter_src->First()), src.m_Field, src.m_Object);
        DataPointer dp_src_value(&iter_src->Second(), src.m_Field, src.m_Object);

        Map<KeyT, ValueT>::ValueType temp;

        DataPointer dp_dest_key(const_cast<ValueT *>(&temp.First()), dest.m_Field, dest.m_Object);
        DataPointer dp_dest_value(&temp.Second(), dest.m_Field, dest.m_Object);

        m_InternalDataKey->Copy(dp_src_key, dp_dest_key, flags);
        m_InternalDataValue->Copy(dp_src_value, dp_dest_value, flags);

        m_dest.Insert(temp);
    }
}

template <class KeyT, class ValueT>
bool Helium::Reflect::SimpleMapData<KeyT, ValueT>::Equals( DataPointer a, DataPointer b ) 
{
    return DefaultEquals< Map< KeyT, ValueT > >(a, b);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::Accept( DataPointer p, Visitor& visitor ) 
{
    Map<KeyT, ValueT> &m = p.As< Map<KeyT, ValueT> >();

    for (Map<KeyT, ValueT>::Iterator iter = m.Begin();
        iter != m.End(); ++iter)
    {
        m_InternalDataKey->Accept(DataPointer(const_cast<KeyT *>(&iter->First()), p.m_Field, p.m_Object), visitor);
        m_InternalDataValue->Accept(DataPointer(&iter->Second(), p.m_Field, p.m_Object), visitor);
    }
}

template <class KeyT, class ValueT>
size_t Helium::Reflect::SimpleMapData<KeyT, ValueT>::GetLength( DataPointer container ) const 
{
    Map<KeyT, ValueT> &m = container.As< Map<KeyT, ValueT> >();
    return m.GetSize();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::Clear( DataPointer container ) 
{
    Map<KeyT, ValueT> &m = container.As< Map<KeyT, ValueT> >();
    return m.Clear();
}

template <class KeyT, class ValueT>
Helium::Reflect::Data* Helium::Reflect::SimpleMapData<KeyT, ValueT>::GetKeyData() const
{
    return m_InternalDataKey;
}

template <class KeyT, class ValueT>
Helium::Reflect::Data* Helium::Reflect::SimpleMapData<KeyT, ValueT>::GetValueData() const
{
    return m_InternalDataValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::GetItems( DataPointer association, DynamicArray<DataPointer>& keys, DynamicArray<DataPointer>& values )
{
    Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();

    for (Map<KeyT, ValueT>::Iterator iter = m.Begin();
        iter != m.End(); ++iter)
    {
        keys.Add(DataPointer(const_cast<KeyT *>(&iter->First()), association.m_Field, association.m_Object));
        values.Add(DataPointer(&iter->Second(), association.m_Field, association.m_Object));
    }
}

template <class KeyT, class ValueT>
Helium::Reflect::DataPointer Helium::Reflect::SimpleMapData<KeyT, ValueT>::GetItem( DataPointer association, DataPointer key )
{
    Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();
    return DataPointer(&m[key.As<KeyT>()], association.m_Field, association.m_Object);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::SetItem( DataPointer association, DataPointer key, DataPointer value )
{
    Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();
    m[key.As<KeyT>()] = value.As<ValueT>();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapData<KeyT, ValueT>::RemoveItem( DataPointer association, DataPointer key )
{
    Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();
    Map<KeyT, ValueT>::Iterator iter = m.Find(key.As<KeyT>());

    if (iter != m.End())
    {
#if 0
        DataPointer dp_key(const_cast<KeyT *>(&iter->First()), association.m_Field, association.m_Object);
        DataPointer dp_value(&iter->Second(), association.m_Field, association.m_Object);

        m_InternalDataKey->Destruct(dp_key);
        m_InternalDataValue->Destruct(dp_vlaue);
#endif

        m.Remove(iter);
    }
}




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
			throw Reflect::Exception( TXT( "SortedSet value type has changed, this is unpossible" ) );
		}

		KeyT k;
		Data::GetValue( data, k );
		m_Data->Insert( k );
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
		throw Reflect::Exception( TXT( "Unmatched map objects" ) );
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
