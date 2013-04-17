#include "ReflectPch.h"
#include "Reflect/Composite.h"

#include "Foundation/Log.h"

#include "Reflect/Data.h"
#include "Reflect/Object.h"
#include "Reflect/Registry.h"
#include "Reflect/Enumeration.h"
#include "Reflect/DataDeduction.h"

using namespace Helium;
using namespace Helium::Reflect;

Field::Field()
: m_Composite( NULL )
, m_Name( NULL )
, m_Size( 0 )
, m_Count( 1 )
, m_Offset( 0 )
, m_Flags( 0 )
, m_Index( ~0 )
, m_Data( NULL )
{

}

bool Field::IsDefaultValue( void* address, Object* object, uint32_t index ) const
{
	DataPointer value ( this, address, object, index );
	DataPointer default ( this, m_Composite->m_Default, object, index );
	return m_Data->Equals( value, default );
}

bool Field::ShouldSerialize( void* address, Object* object, uint32_t index ) const
{
	// never write discard fields
	if ( m_Flags & FieldFlags::Discard )
	{
		return false;
	}

	// always write force fields
	if ( m_Flags & FieldFlags::Force )
	{
		return true;
	}

	return !IsDefaultValue( address, object, index );
}

Composite::Composite()
	: m_Base( NULL )
	, m_FirstDerived( NULL )
	, m_NextSibling( NULL )
	, m_Populate( NULL )
	, m_Default( NULL )
{

}

Composite::~Composite()
{
	delete m_Default;
}

Composite* Composite::Create()
{
	return new Composite();
}

void Composite::Register() const
{
	Type::Register();

	uint32_t computedSize = 0;
	DynamicArray< Field >::ConstIterator itr = m_Fields.Begin();
	DynamicArray< Field >::ConstIterator end = m_Fields.End();
	for ( ; itr != end; ++itr )
	{
		computedSize += itr->m_Size;
		Log::Debug( TXT( "  Index: %3d, Size %4d, Name: %s\n" ), itr->m_Index, itr->m_Size, itr->m_Name );
	}

	if (computedSize != m_Size)
	{
		Log::Debug( TXT( " %d bytes of hidden fields and padding\n" ), m_Size - computedSize );
	}
}

void Composite::Unregister() const
{
	Type::Unregister();
}

bool Composite::IsType(const Composite* type) const
{
	for ( const Composite* base = this; base; base = base->m_Base )
	{
		if ( base == type )
		{
			return true;
		}
	}

	return false;
}

void Composite::AddDerived( const Composite* derived ) const
{
	HELIUM_ASSERT( derived );

	derived->m_NextSibling = m_FirstDerived;
	m_FirstDerived = derived;
}

void Composite::RemoveDerived( const Composite* derived ) const
{
	HELIUM_ASSERT( derived );

	if ( m_FirstDerived == derived )
	{
		m_FirstDerived = derived->m_NextSibling;
	}
	else
	{
		for ( const Composite* sibling = m_FirstDerived; sibling; sibling = sibling->m_NextSibling )
		{
			if ( sibling->m_NextSibling == derived )
			{
				sibling->m_NextSibling = derived->m_NextSibling;
				break;
			}
		}
	}

	derived->m_NextSibling = NULL;
}

bool Composite::Equals(void* compositeA, Object* objectA, void* compositeB, Object* objectB) const
{
	if (compositeA == compositeB)
	{
		return true;
	}

	if (!compositeA || !compositeB)
	{
		return false;
	}

	for ( const Composite* current = this; current != NULL; current = current->m_Base )
	{
		DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
		DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
		for ( ; itr != end; ++itr )
		{
			const Field* field = &*itr;
			DataPointer a ( field, compositeA, objectA );
			DataPointer b ( field, compositeB, objectB );
			bool equality = field->m_Data->Equals( a, b );
			if ( !equality )
			{
				return false;
			}
		}
	}

	return true;
}

void Composite::Visit(void* composite, Object* object, Visitor& visitor) const
{
	if (!composite)
	{
		return;
	}

	for ( const Composite* current = this; current != NULL; current = current->m_Base )
	{
		DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
		DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
		for ( ; itr != end; ++itr )
		{
			const Field* field = &*itr;

			field->m_Data->Accept( DataPointer ( field, composite, object ), visitor );
		}
	}
}

void Composite::Copy( void* compositeSource, Object* objectSource, void* compositeDestination, Object* objectDestination ) const
{
	if ( compositeSource != compositeDestination )
	{
		for ( const Composite* current = this; current != NULL; current = current->m_Base )
		{
			DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
			DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
			for ( ; itr != end; ++itr )
			{
				const Field* field = &*itr;
				DataPointer pointerSource ( field, compositeSource, objectSource );
				DataPointer pointerDestination ( field, compositeDestination, objectDestination );

				// for normal data types, run overloaded assignement operator via data's vtable
				// for reference container types, this deep copies containers (which is bad for 
				//  non-cloneable (FieldFlags::Share) reference containers)
				bool result = field->m_Data->Copy(pointerSource, pointerDestination, field->m_Flags & FieldFlags::Share ? DataFlags::Shallow : 0);
				HELIUM_ASSERT(result);
			}
		}
	}
}

const Field* Composite::FindFieldByName(uint32_t crc) const
{
	for ( const Composite* current = this; current != NULL; current = current->m_Base )
	{
		DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
		DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
		for ( ; itr != end; ++itr )
		{
			if ( Crc32( itr->m_Name ) == crc )
			{
				return &*itr;
			}
		}
	}

	return NULL;
}

const Field* Composite::FindFieldByIndex(uint32_t index) const
{
	for ( const Composite* current = this; current != NULL; current = current->m_Base )
	{
		if ( current->m_Fields.GetSize() && index >= current->m_Fields.GetFirst().m_Index && index <= current->m_Fields.GetFirst().m_Index )
		{
			return &current->m_Fields[ index - current->m_Fields.GetFirst().m_Index ];
		}
	}

	return NULL;
}

const Field* Composite::FindFieldByOffset(uint32_t offset) const
{
#pragma TODO("Implement binary search")
	for ( const Composite* current = this; current != NULL; current = current->m_Base )
	{
		if ( current->m_Fields.GetSize() && offset >= current->m_Fields.GetFirst().m_Offset && offset <= current->m_Fields.GetFirst().m_Offset )
		{
			DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
			DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
			for ( ; itr != end; ++itr )
			{
				if ( itr->m_Offset == offset )
				{
					return &*itr;
				}
			}
		}
	}

	return NULL;
}

uint32_t Composite::GetBaseFieldCount() const
{
	uint32_t count = 0;

	for ( const Composite* base = m_Base; base; base = base->m_Base )
	{
		if ( m_Base->m_Fields.GetSize() )
		{
			count = m_Base->m_Fields.GetLast().m_Index + 1;
			break;
		}
	}

	return count;
}

Reflect::Field* Composite::AddField( const tchar_t* name, uint32_t size, uint32_t count, uint32_t offset, uint32_t flags, Data* data )
{
	// deduction of the data class has failed, you must provide one yourself!
	HELIUM_ASSERT( data );

	Field field;
	field.m_Composite = this;
	field.m_Name = name;
	field.m_Size = size;
	field.m_Offset = offset;
	field.m_Flags = flags;
	field.m_Index = GetBaseFieldCount() + (uint32_t)m_Fields.GetSize();
	field.m_Data = data;
	m_Fields.Add( field );

	return &m_Fields.GetLast();
}
