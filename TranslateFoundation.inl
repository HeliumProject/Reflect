template <class T>
Helium::Reflect::SimpleDynamicArrayTranslator<T>::SimpleDynamicArrayTranslator()
	: SequenceTranslator(sizeof(DynamicArray<T>))
	, m_InternalTranslator(AllocateTranslator<T>())
{

}

template <class T>
Helium::Reflect::SimpleDynamicArrayTranslator<T>::~SimpleDynamicArrayTranslator()
{
	delete m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::Construct( Pointer pointer )
{
	DefaultConstruct< DynamicArray<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct< DynamicArray<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags /*= 0 */ )
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< DynamicArray<T> >(src, dest, flags);
		return;
	}

	DynamicArray<T> &v_src = src.As< DynamicArray<T> >();
	DynamicArray<T> &v_dest = dest.As< DynamicArray<T> >();

	v_dest.Resize(v_src.GetSize());

	typename DynamicArray<T>::Iterator iter_src = v_src.Begin();
	typename DynamicArray<T>::Iterator iter_dest = v_dest.Begin();
	for (; iter_src != v_src.End(); ++iter_src, ++iter_dest)
	{
		Pointer dp_src(&*iter_src, src.m_Field, src.m_Object);
		Pointer dp_dest(&*iter_dest, dest.m_Field, dest.m_Object);
		m_InternalTranslator->Copy(dp_src, dp_dest, flags);
	}
}

template <class T>
bool Helium::Reflect::SimpleDynamicArrayTranslator<T>::Equals( Pointer a, Pointer b )
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
		Pointer dp_a(&*iter_a, a.m_Field, b.m_Object);
		Pointer dp_b(&*iter_b, b.m_Field, b.m_Object);

		if (!m_InternalTranslator->Equals(dp_a, dp_b))
		{
			return false;
		}
	}

	return true;
#endif

	return DefaultEquals< DynamicArray<T> >(a, b);
}

template <class T>
size_t Helium::Reflect::SimpleDynamicArrayTranslator<T>::GetLength( Pointer container ) const
{
	DynamicArray<T> &v = container.As< DynamicArray<T> >();
	return v.GetSize();
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::Clear( Pointer container )
{
	DynamicArray<T> &v = container.As< DynamicArray<T> >();
	v.Clear();
}

template <class T>
Helium::Reflect::Translator* Helium::Reflect::SimpleDynamicArrayTranslator<T>::GetItemTranslator() const
{
	return m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::GetItems( Pointer sequence, DynamicArray< Pointer >& items ) const
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();

	items.Reserve(v.GetSize());

	for ( typename DynamicArray<T>::Iterator iter = v.Begin(); iter != v.End(); ++iter )
	{
		items.Add(Pointer(&*iter, sequence.m_Field, sequence.m_Object));
	}
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::SetLength( Pointer sequence, size_t length )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	v.Resize(length);
}

template <class T>
Helium::Reflect::Pointer Helium::Reflect::SimpleDynamicArrayTranslator<T>::GetItem( Pointer sequence, size_t at )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	return Pointer(&v[at], sequence.m_Field, sequence.m_Object);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::SetItem( Pointer sequence, size_t at, Pointer value )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	m_InternalTranslator->Copy(value, Pointer(&v[at], sequence.m_Field, sequence.m_Object));
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::Insert( Pointer sequence, size_t at, Pointer value )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	v.Insert(at, value.As<T>());
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::Remove( Pointer sequence, size_t at )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();
	v.Remove(at);
}

template <class T>
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::MoveUp( Pointer sequence, Set< size_t >& items )
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
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::MoveDown( Pointer sequence, Set< size_t >& items )
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
void Helium::Reflect::SimpleDynamicArrayTranslator<T>::SwapInternalValues( Pointer sequence, size_t a, size_t b )
{
	DynamicArray<T> &v = sequence.As< DynamicArray<T> >();

	HELIUM_ASSERT(a >= 0);
	HELIUM_ASSERT(b >= 0);

	HELIUM_ASSERT(a < v.GetSize());
	HELIUM_ASSERT(b < v.GetSize());

	HELIUM_ASSERT(a != b);

	T temp;

	typename DynamicArray<T>::Iterator iter_a = v.Begin() + a;
	typename DynamicArray<T>::Iterator iter_b = v.Begin() + b;

	Pointer dp_a(&*iter_a, sequence.m_Field, sequence.m_Object);
	Pointer dp_b(&*iter_b, sequence.m_Field, sequence.m_Object);
	Pointer dp_temp(&temp, sequence.m_Field, sequence.m_Object);

	m_InternalTranslator->Copy(dp_a, dp_temp);
	m_InternalTranslator->Copy(dp_b, dp_a);
	m_InternalTranslator->Copy(dp_temp, dp_b, CopyFlags::Notify);
}

//////////////////////////////////////////////////////////////////////////

template <class T>
Helium::Reflect::SimpleSetTranslator<T>::SimpleSetTranslator()
	: SetTranslator(sizeof(Set<T>))
	, m_InternalTranslator(AllocateTranslator<T>())
{

}

template <class T>
Helium::Reflect::SimpleSetTranslator<T>::~SimpleSetTranslator()
{
	delete m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleSetTranslator<T>::Construct( Pointer pointer )
{
	DefaultConstruct< Set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleSetTranslator<T>::Destruct( Pointer pointer )
{
	DefaultDestruct< Set<T> >(pointer);
}

template <class T>
void Helium::Reflect::SimpleSetTranslator<T>::Copy( Pointer src, Pointer dest, uint32_t flags /*= 0 */ )
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< Set< T > >(src, dest, flags);
		return;
	}

	Set<T> &s_src = src.As< Set<T> >();
	Set<T> &s_dest = dest.As< Set<T> >();

	s_dest.Clear();

	for ( typename Set<T>::Iterator iter_src = s_src.Begin(); iter_src != s_src.End(); ++iter_src )
	{
		// Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
		Pointer dp_src(const_cast<T *>(&*iter_src), src.m_Field, src.m_Object);
		T temp;
		Pointer dp_dest(&temp, dest.m_Field, dest.m_Object);
		m_InternalTranslator->Copy(dp_src, dp_dest, flags);
		s_dest.Insert(temp);
	}
}

template <class T>
bool Helium::Reflect::SimpleSetTranslator<T>::Equals( Pointer a, Pointer b )
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
		Pointer dp_a(const_cast<T *>(&*iter_a), a.m_Field, b.m_Object);
		Pointer dp_b(const_cast<T *>(&*iter_b), b.m_Field, b.m_Object);

		if (!m_InternalTranslator->Equals(dp_a, dp_b))
		{
			return false;
		}
	}

	return true;
#endif

	return DefaultEquals< Set<T> >(a, b);
}

template <class T>
size_t Helium::Reflect::SimpleSetTranslator<T>::GetLength( Pointer container ) const
{
	Set<T> &s = container.As< Set<T> >();
	return s.GetSize();
}

template <class T>
void Helium::Reflect::SimpleSetTranslator<T>::Clear( Pointer container )
{
	Set<T> &s = container.As< Set<T> >();
	s.Clear();
}

template <class T>
Helium::Reflect::Translator* Helium::Reflect::SimpleSetTranslator<T>::GetItemTranslator() const
{
	return m_InternalTranslator;
}

template <class T>
void Helium::Reflect::SimpleSetTranslator<T>::GetItems( Pointer set, DynamicArray< Pointer >& items ) const
{
	Set<T> &v = set.As< Set<T> >();
	items.Reserve(v.GetSize());

	for ( typename Set<T>::Iterator iter = v.Begin(); iter != v.End(); ++iter )
	{
		// This is dangerous.. callers could modify values passed out
		Pointer dp(const_cast<T *>(&*iter), set.m_Field, set.m_Object);
		items.Add(dp);
	}
}

template <class T>
void Helium::Reflect::SimpleSetTranslator<T>::InsertItem( Pointer set, Pointer item )
{
	// Not crazy about this insert going through the copy ctor instead of our copy implementation for the data type
	Set<T> &v = set.As< Set<T> >();
	v.Insert( typename Set<T>::ValueType(item.As<T>()) );
}

template <class T>
void Helium::Reflect::SimpleSetTranslator<T>::RemoveItem( Pointer set, Pointer item )
{
	// Not crazy about this insert going through the c++ equal operator instead of the data type
	Set<T> &v = set.As< Set<T> >();
	v.Remove( typename Set<T>::ValueType(item.As<T>()) );
}

template <class T>
bool Helium::Reflect::SimpleSetTranslator<T>::ContainsItem( Pointer set, Pointer item ) const
{
	Set<T> &s = set.As< Set<T> >();

#if 0
	// This is an awful linear search instead of a log(n) search so that we can defer to the internal data class
	for (Set<T>::Iterator iter = s.Begin(); iter != s.End(); ++iter)
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

	typename Set<T>::Iterator iter = s.Find(item.As<T>());
	return iter != s.End();
}

//////////////////////////////////////////////////////////////////////////

template <class KeyT, class ValueT>
Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::SimpleMapTranslator()
	: AssociationTranslator(sizeof(Map<KeyT, ValueT>))
	, m_InternalTranslatorKey(ReflectionCast< ScalarTranslator >( AllocateTranslator<KeyT>() ))
	, m_InternalTranslatorValue(AllocateTranslator<ValueT>())
{
	HELIUM_ASSERT( m_InternalTranslatorKey );
}

template <class KeyT, class ValueT>
Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::~SimpleMapTranslator()
{
	delete m_InternalTranslatorKey;
	delete m_InternalTranslatorValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::Construct( Pointer pointer ) 
{
	DefaultConstruct< Map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::Destruct( Pointer pointer ) 
{
	DefaultDestruct< Map<KeyT, ValueT> >(pointer);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::Copy( Pointer src, Pointer dest, uint32_t flags /*= 0 */ ) 
{
	if (flags & CopyFlags::Shallow)
	{
		DefaultCopy< Map< KeyT, ValueT > >(src, dest, flags);
		return;
	}

	Map<KeyT, ValueT> &m_src = src.As< Map<KeyT, ValueT> >();
	Map<KeyT, ValueT> &m_dest = dest.As< Map<KeyT, ValueT> >();

	m_dest.Clear();

	for ( typename Map<KeyT, ValueT>::Iterator iter_src = m_src.Begin(); iter_src != m_src.End(); ++iter_src )
	{
		// Should be safe since we copy FROM this. Should not break const-ness (might increase a ref count or something like that though)
		Pointer dp_src_key(const_cast<KeyT *>(&iter_src->First()), src.m_Field, src.m_Object);
		Pointer dp_src_value(&iter_src->Second(), src.m_Field, src.m_Object);

		typename Map<KeyT, ValueT>::ValueType temp;

		Pointer dp_dest_key(const_cast<ValueT *>(&temp.First()), dest.m_Field, dest.m_Object);
		Pointer dp_dest_value(&temp.Second(), dest.m_Field, dest.m_Object);

		m_InternalTranslatorKey->Copy(dp_src_key, dp_dest_key, flags);
		m_InternalTranslatorValue->Copy(dp_src_value, dp_dest_value, flags);

		m_dest.Insert(temp);
	}
}

template <class KeyT, class ValueT>
bool Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::Equals( Pointer a, Pointer b ) 
{
	return DefaultEquals< Map< KeyT, ValueT > >(a, b);
}

template <class KeyT, class ValueT>
size_t Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::GetLength( Pointer container ) const 
{
	Map<KeyT, ValueT> &m = container.As< Map<KeyT, ValueT> >();
	return m.GetSize();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::Clear( Pointer container ) 
{
	Map<KeyT, ValueT> &m = container.As< Map<KeyT, ValueT> >();
	return m.Clear();
}

template <class KeyT, class ValueT>
Helium::Reflect::ScalarTranslator* Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::GetKeyTranslator() const
{
	return m_InternalTranslatorKey;
}

template <class KeyT, class ValueT>
Helium::Reflect::Translator* Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::GetValueTranslator() const
{
	return m_InternalTranslatorValue;
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::GetItems( Pointer association, DynamicArray<Pointer>& keys, DynamicArray<Pointer>& values )
{
	Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();

	for ( typename Map<KeyT, ValueT>::Iterator iter = m.Begin(); iter != m.End(); ++iter )
	{
		keys.Add(Pointer(const_cast<KeyT *>(&iter->First()), association.m_Field, association.m_Object));
		values.Add(Pointer(&iter->Second(), association.m_Field, association.m_Object));
	}
}

template <class KeyT, class ValueT>
Helium::Reflect::Pointer Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::GetItem( Pointer association, Pointer key )
{
	Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();
	return Pointer(&m[key.As<KeyT>()], association.m_Field, association.m_Object);
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::SetItem( Pointer association, Pointer key, Pointer value )
{
	Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();
	m[key.As<KeyT>()] = value.As<ValueT>();
}

template <class KeyT, class ValueT>
void Helium::Reflect::SimpleMapTranslator<KeyT, ValueT>::RemoveItem( Pointer association, Pointer key )
{
	Map<KeyT, ValueT> &m = association.As< Map<KeyT, ValueT> >();
	typename Map<KeyT, ValueT>::Iterator iter = m.Find(key.As<KeyT>());

	if (iter != m.End())
	{
#if 0
		Pointer dp_key(const_cast<KeyT *>(&iter->First()), association.m_Field, association.m_Object);
		Pointer dp_value(&iter->Second(), association.m_Field, association.m_Object);

		m_InternalTranslatorKey->Destruct(dp_key);
		m_InternalTranslatorValue->Destruct(dp_vlaue);
#endif

		m.Remove(iter);
	}
}
