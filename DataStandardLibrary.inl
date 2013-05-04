template <class T>
Helium::Reflect::SimpleStlVectorData<T>::SimpleStlVectorData()
	: SequenceData(sizeof(std::vector<T>))
	, m_InternalData(AllocateData<T>())
{

}

template <class T>
Helium::Reflect::SimpleStlVectorData<T>::~SimpleStlVectorData()
{
	delete m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::Construct( DataPointer pointer )
{
	DefaultConstruct< std::vector<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::Destruct( DataPointer pointer )
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
#if 0
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
#endif

	return DefaultEquals< std::vector<T> >(a, b);
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
Helium::Reflect::Data* Helium::Reflect::SimpleStlVectorData<T>::GetItemData() const
{
	return m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::GetItems( DataPointer sequence, DynamicArray< DataPointer >& items ) const
{
	std::vector<T> &v = sequence.As< std::vector<T> >();

	items.Reserve(v.size());

	for (std::vector<T>::iterator iter = v.begin(); iter != v.end(); ++iter)
	{
		items.Add(DataPointer(&*iter, sequence.m_Field, sequence.m_Object));
	}
}

template <class T>
void Helium::Reflect::SimpleStlVectorData<T>::SetLength( DataPointer sequence, size_t length )
{
	std::vector<T> &v = sequence.As< std::vector<T> >();
	v.resize(length);
}

template <class T>
Helium::Reflect::DataPointer Helium::Reflect::SimpleStlVectorData<T>::GetItem( DataPointer sequence, size_t at )
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
	: SetData(sizeof(std::set<T>))
	, m_InternalData(AllocateData<T>())
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
#if 0
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
#endif

	return DefaultEquals< std::set<T> >(a, b);
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
Helium::Reflect::Data* Helium::Reflect::SimpleStlSetData<T>::GetItemData() const
{
	return m_InternalData;
}

template <class T>
void Helium::Reflect::SimpleStlSetData<T>::GetItems( DataPointer set, DynamicArray< DataPointer >& items ) const
{
	std::set<T> &v = set.As< std::set<T> >();
	items.Reserve(v.size());

	for (std::set<T>::iterator iter = v.begin(); iter != v.end(); ++iter)
	{
		// This is dangerous.. callers could modify values passed out
		DataPointer dp(const_cast<T *>(&*iter), set.m_Field, set.m_Object);
		items.Add(dp);
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

#if 0
	// This is an awful linear search instead of a log(n) search so that we can defer to the internal data class
	for (std::set<T>::iterator iter = s.begin(); iter != s.end(); ++iter)
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

	std::set<T>::iterator iter = s.find(item.As<T>());
	return iter != s.end();
}

//////////////////////////////////////////////////////////////////////////

template <class KeyT, class ValueT>
Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::SimpleStlMapData()
	: AssociationData(sizeof(std::map<KeyT, ValueT>))
	, m_InternalDataKey(AllocateData<KeyT>())
	, m_InternalDataValue(AllocateData<ValueT>())
{

}

template <class KeyT, class ValueT>
Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::~SimpleStlMapData()
{
	delete m_InternalDataKey;
	delete m_InternalDataValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::Construct( DataPointer pointer ) 
{
	DefaultConstruct< std::map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::Destruct( DataPointer pointer ) 
{
	DefaultDestruct< std::map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::Copy( DataPointer src, DataPointer dest, uint32_t flags /*= 0 */ ) 
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< std::map< KeyT, ValueT > >(src, dest, flags);
		return;
	}

	std::map<KeyT, ValueT> &m_src = src.As< std::map<KeyT, ValueT> >();
	std::map<KeyT, ValueT> &m_dest = dest.As< std::map<KeyT, ValueT> >();

	m_dest.clear();

	for (std::map<KeyT, ValueT>::iterator iter_src = m_src.begin();
		iter_src != m_src.end(); ++iter_src)
	{
		// Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
		DataPointer dp_src_key(const_cast<KeyT *>(&iter_src->first), src.m_Field, src.m_Object);
		DataPointer dp_src_value(&iter_src->second, src.m_Field, src.m_Object);

		std::map<KeyT, ValueT>::value_type temp;

		DataPointer dp_dest_key(const_cast<ValueT *>(&temp.first), dest.m_Field, dest.m_Object);
		DataPointer dp_dest_value(&temp.second, dest.m_Field, dest.m_Object);

		m_InternalDataKey->Copy(dp_src_key, dp_dest_key, flags);
		m_InternalDataValue->Copy(dp_src_value, dp_dest_value, flags);

		m_dest.insert(temp);
	}
}

template <class KeyT, class ValueT>
bool Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::Equals( DataPointer a, DataPointer b ) 
{
	return DefaultEquals< std::map< KeyT, ValueT > >(a, b);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::Accept( DataPointer p, Visitor& visitor ) 
{
	std::map<KeyT, ValueT> &m = p.As< std::map<KeyT, ValueT> >();

	for (std::map<KeyT, ValueT>::iterator iter = m.begin();
		iter != m.end(); ++iter)
	{
		m_InternalDataKey->Accept(DataPointer(const_cast<KeyT *>(&iter->first), p.m_Field, p.m_Object), visitor);
		m_InternalDataValue->Accept(DataPointer(&iter->second, p.m_Field, p.m_Object), visitor);
	}
}

template <class KeyT, class ValueT>
size_t Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::GetLength( DataPointer container ) const 
{
	std::map<KeyT, ValueT> &m = container.As< std::map<KeyT, ValueT> >();
	return m.size();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::Clear( DataPointer container ) 
{
	std::map<KeyT, ValueT> &m = container.As< std::map<KeyT, ValueT> >();
	return m.clear();
}

template <class KeyT, class ValueT>
Helium::Reflect::Data* Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::GetKeyData() const
{
	return m_InternalDataKey;
}

template <class KeyT, class ValueT>
Helium::Reflect::Data* Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::GetValueData() const
{
	return m_InternalDataValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::GetItems( DataPointer association, DynamicArray<DataPointer>& keys, DynamicArray<DataPointer>& values )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();

	for (std::map<KeyT, ValueT>::iterator iter = m.begin();
		iter != m.end(); ++iter)
	{
		keys.Add(DataPointer(const_cast<KeyT *>(&iter->first), association.m_Field, association.m_Object));
		values.Add(DataPointer(&iter->second, association.m_Field, association.m_Object));
	}
}

template <class KeyT, class ValueT>
Helium::Reflect::DataPointer Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::GetItem( DataPointer association, DataPointer key )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();
	return DataPointer(&m[key.As<KeyT>()], association.m_Field, association.m_Object);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::SetItem( DataPointer association, DataPointer key, DataPointer value )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();
	m[key.As<KeyT>()] = value.As<ValueT>();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapData<KeyT, ValueT>::RemoveItem( DataPointer association, DataPointer key )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();
	std::map<KeyT, ValueT>::iterator iter = m.find(key.As<KeyT>());

	if (iter != m.end())
	{
#if 0
		DataPointer dp_key(const_cast<KeyT *>(&iter->first), association.m_Field, association.m_Object);
		DataPointer dp_value(&iter->second, association.m_Field, association.m_Object);

		m_InternalDataKey->Destruct(dp_key);
		m_InternalDataValue->Destruct(dp_vlaue);
#endif

		m.erase(iter);
	}
}
