template <class T>
Helium::Reflect::SimpleStlVectorTranslator<T>::SimpleStlVectorTranslator()
	: SequenceTranslator(sizeof(std::vector<T>))
	, m_InternalTranslator(AllocateTranslator<T>())
{

}

template <class T>
Helium::Reflect::SimpleStlVectorTranslator<T>::~SimpleStlVectorTranslator()
{
	delete m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::Construct( Pointer pointer )
{
	DefaultConstruct< std::vector<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct< std::vector<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags /*= 0 */ )
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< std::vector<T> >(src, dest, flags);
		return;
	}

	std::vector<T> &v_src = src.As< std::vector<T> >();
	std::vector<T> &v_dest = dest.As< std::vector<T> >();

	v_dest.resize(v_src.size());

	typename std::vector<T>::iterator iter_src = v_src.begin();
	typename std::vector<T>::iterator iter_dest = v_dest.begin();

	for (; iter_src != v_src.end(); ++iter_src, ++iter_dest)
	{
		Pointer dp_src(&*iter_src, src.m_Field, src.m_Object);
		Pointer dp_dest(&*iter_dest, dest.m_Field, dest.m_Object);
		m_InternalTranslator->Copy(dp_src, dp_dest, flags);
	}
}

template <class T>
bool Helium::Reflect::SimpleStlVectorTranslator<T>::Equals( Pointer a, Pointer b )
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
		Pointer dp_a(&*iter_a, a.m_Field, b.m_Object);
		Pointer dp_b(&*iter_b, b.m_Field, b.m_Object);

		if (!m_InternalTranslator->Equals(dp_a, dp_b))
		{
			return false;
		}
	}

	return true;
#endif

	return DefaultEquals< std::vector<T> >(a, b);
}

template <class T>
size_t Helium::Reflect::SimpleStlVectorTranslator<T>::GetLength( Pointer container ) const
{
	std::vector<T> &v = container.As< std::vector<T> >();
	return v.size();
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::Clear( Pointer container )
{
	std::vector<T> &v = container.As< std::vector<T> >();
	v.clear();
}

template <class T>
Helium::Reflect::Translator* Helium::Reflect::SimpleStlVectorTranslator<T>::GetItemTranslator() const
{
	return m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::GetItems( Pointer sequence, DynamicArray< Pointer >& items ) const
{
	std::vector<T> &v = sequence.As< std::vector<T> >();

	items.Reserve(v.size());

	for ( typename std::vector<T>::iterator iter = v.begin(); iter != v.end(); ++iter )
	{
		items.Add(Pointer(&*iter, sequence.m_Field, sequence.m_Object));
	}
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::SetLength( Pointer sequence, size_t length )
{
	std::vector<T> &v = sequence.As< std::vector<T> >();
	v.resize(length);
}

template <class T>
Helium::Reflect::Pointer Helium::Reflect::SimpleStlVectorTranslator<T>::GetItem( Pointer sequence, size_t at )
{
	std::vector<T> &v = sequence.As< std::vector<T> >();
	return Pointer(&v[at], sequence.m_Field, sequence.m_Object);
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::SetItem( Pointer sequence, size_t at, Pointer value )
{
	std::vector<T> &v = sequence.As< std::vector<T> >();
	m_InternalTranslator->Copy(value, Pointer(&v[at], sequence.m_Field, sequence.m_Object));
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::Insert( Pointer sequence, size_t at, Pointer value )
{
	std::vector<T> &v = sequence.As< std::vector<T> >();
	v.insert(v.begin() + at, value.As<T>());
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::Remove( Pointer sequence, size_t at )
{
	std::vector<T> &v = sequence.As< std::vector<T> >();
	v.erase(v.begin() + at);
}

template <class T>
void Helium::Reflect::SimpleStlVectorTranslator<T>::MoveUp( Pointer sequence, Set< size_t >& items )
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
void Helium::Reflect::SimpleStlVectorTranslator<T>::MoveDown( Pointer sequence, Set< size_t >& items )
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
void Helium::Reflect::SimpleStlVectorTranslator<T>::SwapInternalValues( Pointer sequence, size_t a, size_t b )
{
	std::vector<T> &v = sequence.As< std::vector<T> >();

	HELIUM_ASSERT(a >= 0);
	HELIUM_ASSERT(b >= 0);

	HELIUM_ASSERT(a < v.size());
	HELIUM_ASSERT(b < v.size());

	HELIUM_ASSERT(a != b);

	T temp;

	typename std::vector<T>::iterator iter_a = v.begin() + a;
	typename std::vector<T>::iterator iter_b = v.begin() + b;

	Pointer dp_a(&*iter_a, sequence.m_Field, sequence.m_Object);
	Pointer dp_b(&*iter_b, sequence.m_Field, sequence.m_Object);
	Pointer dp_temp(&temp, sequence.m_Field, sequence.m_Object);

	m_InternalTranslator->Copy(dp_a, dp_temp);
	m_InternalTranslator->Copy(dp_b, dp_a);
	m_InternalTranslator->Copy(dp_temp, dp_b, CopyFlags::Notify);
}

//////////////////////////////////////////////////////////////////////////

template <class T>
Helium::Reflect::SimpleStlSetTranslator<T>::SimpleStlSetTranslator()
	: SetTranslator(sizeof(std::set<T>))
	, m_InternalTranslator(AllocateTranslator<T>())
{

}

template <class T>
Helium::Reflect::SimpleStlSetTranslator<T>::~SimpleStlSetTranslator()
{
	delete m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleStlSetTranslator<T>::Construct( Pointer pointer )
{
	DefaultConstruct< std::set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlSetTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct< std::set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleStlSetTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags /*= 0 */ )
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< std::set< T > >(src, dest, flags);
		return;
	}

	std::set<T> &s_src = src.As< std::set<T> >();
	std::set<T> &s_dest = dest.As< std::set<T> >();

	s_dest.clear();

	for ( typename std::set<T>::iterator iter_src = s_src.begin(); iter_src != s_src.end(); ++iter_src)
	{
		// Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
		Pointer dp_src(const_cast<T *>(&*iter_src), src.m_Field, src.m_Object);
		T temp;
		Pointer dp_dest(&temp, dest.m_Field, dest.m_Object);
		m_InternalTranslator->Copy(dp_src, dp_dest, flags);
		s_dest.insert(temp);
	}
}

template <class T>
bool Helium::Reflect::SimpleStlSetTranslator<T>::Equals( Pointer a, Pointer b )
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
		Pointer dp_a(const_cast<T *>(&*iter_a), a.m_Field, b.m_Object);
		Pointer dp_b(const_cast<T *>(&*iter_b), b.m_Field, b.m_Object);

		if (!m_InternalTranslator->Equals(dp_a, dp_b))
		{
			return false;
		}
	}

	return true;
#endif

	return DefaultEquals< std::set<T> >(a, b);
}

template <class T>
size_t Helium::Reflect::SimpleStlSetTranslator<T>::GetLength( Pointer container ) const
{
	std::set<T> &s = container.As< std::set<T> >();
	return s.size();
}

template <class T>
void Helium::Reflect::SimpleStlSetTranslator<T>::Clear( Pointer container )
{
	std::set<T> &s = container.As< std::set<T> >();
	s.clear();
}

template <class T>
Helium::Reflect::Translator* Helium::Reflect::SimpleStlSetTranslator<T>::GetItemTranslator() const
{
	return m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleStlSetTranslator<T>::GetItems( Pointer set, DynamicArray< Pointer >& items ) const
{
	std::set<T> &v = set.As< std::set<T> >();
	items.Reserve(v.size());

	for ( typename std::set<T>::iterator iter = v.begin(); iter != v.end(); ++iter )
	{
		// This is dangerous.. callers could modify values passed out
		Pointer dp(const_cast<T *>(&*iter), set.m_Field, set.m_Object);
		items.Add(dp);
	}
}

template <class T>
void Helium::Reflect::SimpleStlSetTranslator<T>::InsertItem( Pointer set, Pointer item )
{
	// Not crazy about this insert going through the copy ctor instead of our copy implementation for the data type
	std::set<T> &v = set.As< std::set<T> >();
	v.insert( typename std::set<T>::value_type(item.As<T>()) );
}

template <class T>
void Helium::Reflect::SimpleStlSetTranslator<T>::RemoveItem( Pointer set, Pointer item )
{
	// Not crazy about this insert going through the c++ equal operator instead of the data type
	std::set<T> &v = set.As< std::set<T> >();
	v.erase( typename std::set<T>::value_type(item.As<T>()) );
}

template <class T>
bool Helium::Reflect::SimpleStlSetTranslator<T>::ContainsItem( Pointer set, Pointer item ) const
{
	std::set<T> &s = set.As< std::set<T> >();

#if 0
	// This is an awful linear search instead of a log(n) search so that we can defer to the internal data class
	for (std::set<T>::iterator iter = s.begin(); iter != s.end(); ++iter)
	{
		// Should be safe since we are checking equality
		Pointer a(const_cast<T *>(&*iter), set.m_Field, set.m_Object);

		if (m_InternalTranslator->Equals(a, item))
		{
			return true;
		}
	}
	
	return false;
#endif

	typename std::set<T>::iterator iter = s.find(item.As<T>());
	return iter != s.end();
}

//////////////////////////////////////////////////////////////////////////

template <class KeyT, class ValueT>
Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::SimpleStlMapTranslator()
	: AssociationTranslator(sizeof(std::map<KeyT, ValueT>))
	, m_InternalTranslatorKey(ReflectionCast< ScalarTranslator >( AllocateTranslator<KeyT>() ))
	, m_InternalTranslatorValue(AllocateTranslator<ValueT>())
{
	HELIUM_ASSERT( m_InternalTranslatorKey );
}

template <class KeyT, class ValueT>
Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::~SimpleStlMapTranslator()
{
	delete m_InternalTranslatorKey;
	delete m_InternalTranslatorValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::Construct( Pointer pointer ) 
{
	DefaultConstruct< std::map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::Destruct( Pointer pointer ) 
{
	DefaultDestruct< std::map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::Copy( Pointer src, Pointer dest, uint32_t flags /*= 0 */ ) 
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< std::map< KeyT, ValueT > >(src, dest, flags);
		return;
	}

	std::map<KeyT, ValueT> &m_src = src.As< std::map<KeyT, ValueT> >();
	std::map<KeyT, ValueT> &m_dest = dest.As< std::map<KeyT, ValueT> >();

	m_dest.clear();

	for ( typename std::map<KeyT, ValueT>::iterator iter_src = m_src.begin(); iter_src != m_src.end(); ++iter_src )
	{
		// Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
		Pointer dp_src_key(const_cast<KeyT *>(&iter_src->first), src.m_Field, src.m_Object);
		Pointer dp_src_value(&iter_src->second, src.m_Field, src.m_Object);

		typename std::map<KeyT, ValueT>::value_type temp;

		Pointer dp_dest_key(const_cast<KeyT *>(&temp.first), dest.m_Field, dest.m_Object);
		Pointer dp_dest_value(&temp.second, dest.m_Field, dest.m_Object);

		m_InternalTranslatorKey->Copy(dp_src_key, dp_dest_key, flags);
		m_InternalTranslatorValue->Copy(dp_src_value, dp_dest_value, flags);

		m_dest.insert(temp);
	}
}

template <class KeyT, class ValueT>
bool Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::Equals( Pointer a, Pointer b ) 
{
	return DefaultEquals< std::map< KeyT, ValueT > >(a, b);
}

template <class KeyT, class ValueT>
size_t Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::GetLength( Pointer container ) const 
{
	std::map<KeyT, ValueT> &m = container.As< std::map<KeyT, ValueT> >();
	return m.size();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::Clear( Pointer container ) 
{
	std::map<KeyT, ValueT> &m = container.As< std::map<KeyT, ValueT> >();
	return m.clear();
}

template <class KeyT, class ValueT>
Helium::Reflect::ScalarTranslator* Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::GetKeyTranslator() const
{
	return m_InternalTranslatorKey;
}

template <class KeyT, class ValueT>
Helium::Reflect::Translator* Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::GetValueTranslator() const
{
	return m_InternalTranslatorValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::GetItems( Pointer association, DynamicArray<Pointer>& keys, DynamicArray<Pointer>& values )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();

	for ( typename std::map<KeyT, ValueT>::iterator iter = m.begin(); iter != m.end(); ++iter )
	{
		keys.Add(Pointer(const_cast<KeyT *>(&iter->first), association.m_Field, association.m_Object));
		values.Add(Pointer(&iter->second, association.m_Field, association.m_Object));
	}
}

template <class KeyT, class ValueT>
Helium::Reflect::Pointer Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::GetItem( Pointer association, Pointer key )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();
	return Pointer(&m[key.As<KeyT>()], association.m_Field, association.m_Object);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::SetItem( Pointer association, Pointer key, Pointer value )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();
	m[key.As<KeyT>()] = value.As<ValueT>();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleStlMapTranslator<KeyT, ValueT>::RemoveItem( Pointer association, Pointer key )
{
	std::map<KeyT, ValueT> &m = association.As< std::map<KeyT, ValueT> >();
	typename std::map<KeyT, ValueT>::iterator iter = m.find(key.As<KeyT>());

	if (iter != m.end())
	{
#if 0
		Pointer dp_key(const_cast<KeyT *>(&iter->first), association.m_Field, association.m_Object);
		Pointer dp_value(&iter->second, association.m_Field, association.m_Object);

		m_InternalTranslatorKey->Destruct(dp_key);
		m_InternalTranslatorValue->Destruct(dp_vlaue);
#endif

		m.erase(iter);
	}
}
