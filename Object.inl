template< class FieldT >
void Helium::Reflect::Object::FieldChanged( FieldT* fieldAddress ) const
{
    // the offset of the field is the address of the field minus the address of this object instance
    uintptr_t fieldOffset = ((uintptr_t)fieldAddress - (uintptr_t)this);

    // find the field in our reflection information
    const Reflect::Field* field = GetMetaClass()->FindFieldByOffset( fieldOffset );

    // your field address probably doesn't point to the field in this instance,
    //  or your field is not exposed to Reflect, add it in your MetaStruct function
    HELIUM_ASSERT( field );

    // notify listeners that this field changed
    RaiseChanged( field );
}

template< class ObjectT, class FieldT >
void Helium::Reflect::Object::ChangeField( FieldT ObjectT::* pointerToMember, const FieldT& newValue )
{
    // set the field via pointer-to-member on the deduced templated type (!)
    this->*pointerToMember = newValue;

    // find the field in our reflection information
    const Reflect::Field* field = GetMetaClass()->FindField( pointerToMember );

    // your field is not exposed to Reflect, add it in your MetaStruct function
    HELIUM_ASSERT( field );

    // notify listeners that this field changed
    RaiseChanged( field );
}

/// Perform any pre-destruction work before clearing the last strong reference to an object and destroying the
/// object.
///
/// @param[in] pObject  Object about to be destroyed.
///
/// @see Destroy()
void Helium::Reflect::ObjectRefCountSupport::PreDestroy( Object* pObject )
{
    HELIUM_ASSERT( pObject );

    pObject->RefCountPreDestroy();
}

/// Destroy an object after the final strong reference to it has been cleared.
///
/// @param[in] pObject  Object to destroy.
///
/// @see PreDestroy()
void Helium::Reflect::ObjectRefCountSupport::Destroy( Object* pObject )
{
    HELIUM_ASSERT( pObject );

    pObject->RefCountDestroy();
}

//
// AssertCast type checks in debug and asserts if failure, does no type checking in release
//

template<class DerivedT>
inline DerivedT* Helium::Reflect::AssertCast( Reflect::Object* base )
{
    if ( base != NULL )
    {
        HELIUM_ASSERT( base->IsA( GetMetaClass<DerivedT>() ) );
    }

    return static_cast< DerivedT* >( base );
}

template<class DerivedT>
inline const DerivedT* Helium::Reflect::AssertCast(const Reflect::Object* base)
{
    if ( base != NULL )
    {
        HELIUM_ASSERT( base->IsA( GetMetaClass<DerivedT>() ) );
    }

    return static_cast< const DerivedT* >( base );
}

//
// ThrowCast type checks and throws if failure
//

template<class DerivedT>
inline DerivedT* Helium::Reflect::ThrowCast(Reflect::Object* base)
{
    if ( base != NULL && !base->IsA( GetMetaClass<DerivedT>() ) )
    {
        throw CastException ( "Object of type '%s' cannot be cast to type '%s'", base->GetMetaClass()->m_Name, GetMetaClass<DerivedT>()->m_Name );
    }

    return static_cast< DerivedT* >( base );
}

template<class DerivedT>
inline const DerivedT* Helium::Reflect::ThrowCast(const Reflect::Object* base)
{
    if ( base != NULL && !base->IsA( GetMetaClass<DerivedT>() ) )
    {
        throw CastException ( "Object of type '%s' cannot be cast to type '%s'", base->GetMetaClass()->m_Name, GetMetaClass<DerivedT>()->m_Name );
    }

    return static_cast< const DerivedT* >( base );
}

//
// SafeCast always type checks and returns null if failure
//

template<class DerivedT>
inline DerivedT* Helium::Reflect::SafeCast(Reflect::Object* base)
{
    if ( base != NULL && base->IsA( GetMetaClass<DerivedT>() ) )
    {
        return static_cast< DerivedT* >( base );
    }
    else
    {
        return NULL;
    }
}

template<class DerivedT>
inline const DerivedT* Helium::Reflect::SafeCast(const Reflect::Object* base)
{
    if ( base != NULL && base->IsA( GetMetaClass<DerivedT>() ) )
    {
        return static_cast< const DerivedT* >( base );
    }
    else
    {
        return NULL;
    }
}

template< class T >
bool Helium::Reflect::ObjectResolver::Resolve( const Name& identity, StrongPtr< T >& object )
{
	const MetaClass* pointerClass = Reflect::GetMetaClass< T >();
	return this->Resolve( identity, reinterpret_cast< ObjectPtr& >( object ), pointerClass );
}
