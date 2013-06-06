#include "ReflectPch.h"
#include "Reflect/Structure.h"

#include "Foundation/Log.h"

#include "Reflect/Translator.h"
#include "Reflect/Object.h"
#include "Reflect/Registry.h"
#include "Reflect/Enumeration.h"
#include "Reflect/TranslatorDeduction.h"

using namespace Helium;
using namespace Helium::Reflect;

#pragma TODO("Remove separate base/derived structure macros in favor of registering StructureBase (like Object)")

Field::Field()
: m_Structure( NULL )
, m_Name( NULL )
, m_Size( 0 )
, m_Count( 1 )
, m_Offset( 0 )
, m_Flags( 0 )
, m_Index( ~0 )
, m_KeyType( NULL )
, m_ValueType( NULL )
, m_Translator( NULL )
{

}

bool Field::IsDefaultValue( void* address, Object* object, uint32_t index ) const
{
	Pointer value ( this, address, object, index );
	Pointer defaultValue ( this, m_Structure->m_Default, NULL, index );
	return m_Translator->Equals( value, defaultValue );
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

Structure::Structure()
	: m_Base( NULL )
	, m_FirstDerived( NULL )
	, m_NextSibling( NULL )
	, m_Populate( NULL )
	, m_Default( NULL )
{

}

Structure::~Structure()
{
	delete static_cast< char* >( m_Default );
}

Structure* Structure::Create()
{
	return new Structure();
}

void Structure::Register() const
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

void Structure::Unregister() const
{
	Type::Unregister();
}

bool Structure::IsType(const Structure* type) const
{
	for ( const Structure* base = this; base; base = base->m_Base )
	{
		if ( base == type )
		{
			return true;
		}
	}

	return false;
}

void Structure::AddDerived( const Structure* derived ) const
{
	HELIUM_ASSERT( derived );

	derived->m_NextSibling = m_FirstDerived;
	m_FirstDerived = derived;
}

void Structure::RemoveDerived( const Structure* derived ) const
{
	HELIUM_ASSERT( derived );

	if ( m_FirstDerived == derived )
	{
		m_FirstDerived = derived->m_NextSibling;
	}
	else
	{
		for ( const Structure* sibling = m_FirstDerived; sibling; sibling = sibling->m_NextSibling )
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

bool Structure::Equals(void* compositeA, Object* objectA, void* compositeB, Object* objectB) const
{
	if (compositeA == compositeB)
	{
		return true;
	}

	if (!compositeA || !compositeB)
	{
		return false;
	}

	for ( const Structure* current = this; current != NULL; current = current->m_Base )
	{
		DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
		DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
		for ( ; itr != end; ++itr )
		{
			const Field* field = &*itr;
			Pointer a ( field, compositeA, objectA );
			Pointer b ( field, compositeB, objectB );
			bool equality = field->m_Translator->Equals( a, b );
			if ( !equality )
			{
				return false;
			}
		}
	}

	return true;
}

void Structure::Copy( void* compositeSource, Object* objectSource, void* compositeDestination, Object* objectDestination, bool shallowCopy ) const
{
	if ( compositeSource != compositeDestination )
	{
		for ( const Structure* current = this; current != NULL; current = current->m_Base )
		{
			DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
			DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
			for ( ; itr != end; ++itr )
			{
				const Field* field = &*itr;
				Pointer pointerSource ( field, compositeSource, objectSource );
				Pointer pointerDestination ( field, compositeDestination, objectDestination );

				// for normal data types, run overloaded assignement operator via data's vtable
				// for reference container types, this deep copies containers (which is bad for 
				//  non-cloneable (FieldFlags::Share) reference containers)
				field->m_Translator->Copy(pointerSource, pointerDestination, shallowCopy || (field->m_Flags & FieldFlags::Share) ? CopyFlags::Shallow : 0);
			}
		}
	}
}

const Field* Structure::FindFieldByName(uint32_t crc) const
{
	for ( const Structure* current = this; current != NULL; current = current->m_Base )
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

const Field* Structure::FindFieldByIndex(uint32_t index) const
{
	for ( const Structure* current = this; current != NULL; current = current->m_Base )
	{
		if ( current->m_Fields.GetSize() && index >= current->m_Fields.GetFirst().m_Index && index <= current->m_Fields.GetFirst().m_Index )
		{
			return &current->m_Fields[ index - current->m_Fields.GetFirst().m_Index ];
		}
	}

	return NULL;
}

const Field* Structure::FindFieldByOffset(uint32_t offset) const
{
#pragma TODO("Implement binary search")
	for ( const Structure* current = this; current != NULL; current = current->m_Base )
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

uint32_t Structure::GetBaseFieldCount() const
{
	uint32_t count = 0;

	for ( const Structure* base = m_Base; base; base = base->m_Base )
	{
		if ( m_Base->m_Fields.GetSize() )
		{
			count = m_Base->m_Fields.GetLast().m_Index + 1;
			break;
		}
	}

	return count;
}

Reflect::Field* Structure::AddField()
{
	Field field;
	field.m_Structure = this;
	field.m_Index = GetBaseFieldCount() + (uint32_t)m_Fields.GetSize();
	m_Fields.Add( field );
	return &m_Fields.GetLast();
}
