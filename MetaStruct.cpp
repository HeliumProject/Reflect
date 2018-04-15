#include "Precompile.h"
#include "Reflect/MetaStruct.h"

#include "Foundation/Log.h"

#include "Reflect/Translator.h"
#include "Reflect/Object.h"
#include "Reflect/Registry.h"
#include "Reflect/MetaEnum.h"
#include "Reflect/TranslatorDeduction.h"

using namespace Helium;
using namespace Helium::Reflect;

// TODO: Remove separate base/derived structure macros in favor of registering Struct (like Object)

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

Method::Method()
: m_Structure( NULL )
, m_Name( NULL )
{

}

bool Field::IsDefaultValue( void* address, Object* object, uint32_t index ) const
{
	Pointer value ( this, address, object, index );
	HELIUM_ASSERT( m_Structure->m_Default );
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

MetaStruct::MetaStruct()
	: m_Base( NULL )
	, m_FirstDerived( NULL )
	, m_NextSibling( NULL )
	, m_Populate( NULL )
	, m_Default( NULL )
	, m_DefaultDelete( NULL )
{

}

MetaStruct::~MetaStruct()
{
	if ( m_Default && HELIUM_VERIFY( m_DefaultDelete ) )
	{
		m_DefaultDelete( m_Default );
	}
}

MetaStruct* MetaStruct::Create()
{
	return new MetaStruct();
}

void MetaStruct::Register() const
{
	MetaType::Register();

	uint32_t computedSize = 0;
	DynamicArray< Field >::ConstIterator itr = m_Fields.Begin();
	DynamicArray< Field >::ConstIterator end = m_Fields.End();
	for ( ; itr != end; ++itr )
	{
		computedSize += itr->m_Size;
		Log::Debug( "  Index: %3d, Size %4d, Name: %s\n", itr->m_Index, itr->m_Size, itr->m_Name );
	}

	if (computedSize != m_Size)
	{
		Log::Debug( " %d bytes of hidden fields and padding\n", m_Size - computedSize );
	}
}

void MetaStruct::Unregister() const
{
	MetaType::Unregister();
}

bool MetaStruct::IsType(const MetaStruct* type) const
{
	for ( const MetaStruct* base = this; base; base = base->m_Base )
	{
		if ( base == type )
		{
			return true;
		}
	}

	return false;
}

void MetaStruct::AddDerived( const MetaStruct* derived ) const
{
	HELIUM_ASSERT( derived );

	derived->m_NextSibling = m_FirstDerived;
	m_FirstDerived = derived;
}

void MetaStruct::RemoveDerived( const MetaStruct* derived ) const
{
	HELIUM_ASSERT( derived );

	if ( m_FirstDerived == derived )
	{
		m_FirstDerived = derived->m_NextSibling;
	}
	else
	{
		for ( const MetaStruct* sibling = m_FirstDerived; sibling; sibling = sibling->m_NextSibling )
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

bool MetaStruct::Equals(void* compositeA, Object* objectA, void* compositeB, Object* objectB) const
{
	if (compositeA == compositeB)
	{
		return true;
	}

	if (!compositeA || !compositeB)
	{
		return false;
	}

	for ( const MetaStruct* current = this; current != NULL; current = current->m_Base )
	{
		DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
		DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
		for ( ; itr != end; ++itr )
		{
			const Field* field = &*itr;

			for ( uint32_t i=0; i<field->m_Count; ++i )
			{
				Pointer a ( field, compositeA, objectA, i );
				Pointer b ( field, compositeB, objectB, i );
				bool equality = field->m_Translator->Equals( a, b );
				if ( !equality )
				{
					return false;
				}
			}
		}
	}

	return true;
}

void MetaStruct::Copy( void* compositeSource, Object* objectSource, void* compositeDestination, Object* objectDestination, bool shallowCopy ) const
{
	if ( compositeSource != compositeDestination )
	{
		for ( const MetaStruct* current = this; current != NULL; current = current->m_Base )
		{
			DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
			DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
			for ( ; itr != end; ++itr )
			{
				const Field* field = &*itr;

				for ( uint32_t i=0; i<field->m_Count; ++i )
				{
					Pointer pointerSource ( field, compositeSource, objectSource, i );
					Pointer pointerDestination ( field, compositeDestination, objectDestination, i );

					// for normal data types, run overloaded assignement operator via data's vtable
					// for reference container types, this deep copies containers (which is bad for 
					//  non-cloneable (FieldFlags::Share) reference containers)
					field->m_Translator->Copy(pointerSource, pointerDestination, shallowCopy || (field->m_Flags & FieldFlags::Share) ? CopyFlags::Shallow : 0);
				}
			}
		}
	}
}

const Field* MetaStruct::FindFieldByName(uint32_t crc) const
{
	for ( const MetaStruct* current = this; current != NULL; current = current->m_Base )
	{
		DynamicArray< Field >::ConstIterator itr = current->m_Fields.Begin();
		DynamicArray< Field >::ConstIterator end = current->m_Fields.End();
		for ( ; itr != end; ++itr )
		{
			if ( itr->m_NameCrc == crc )
			{
				return &*itr;
			}
		}
	}

	return NULL;
}

const Field* MetaStruct::FindFieldByIndex(uint32_t index) const
{
	for ( const MetaStruct* current = this; current != NULL; current = current->m_Base )
	{
		if ( current->m_Fields.GetSize() && index >= current->m_Fields.GetFirst().m_Index && index <= current->m_Fields.GetFirst().m_Index )
		{
			return &current->m_Fields[ index - current->m_Fields.GetFirst().m_Index ];
		}
	}

	return NULL;
}

const Field* MetaStruct::FindFieldByOffset(uint32_t offset) const
{
	// TODO: Implement binary search
	for ( const MetaStruct* current = this; current != NULL; current = current->m_Base )
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

uint32_t MetaStruct::GetBaseFieldCount() const
{
	uint32_t count = 0;

	for ( const MetaStruct* base = m_Base; base; base = base->m_Base )
	{
		if ( m_Base->m_Fields.GetSize() )
		{
			count = m_Base->m_Fields.GetLast().m_Index + 1;
			break;
		}
	}

	return count;
}

Reflect::Field* MetaStruct::AllocateField()
{
	Field field;
	field.m_Structure = this;
	field.m_Index = GetBaseFieldCount() + (uint32_t)m_Fields.GetSize();
	m_Fields.Add( field );
	return &m_Fields.GetLast();
}

Reflect::Method* MetaStruct::AllocateMethod()
{
	Method method;
	method.m_Structure = this;
	m_Methods.Add( method );
	return &m_Methods.GetLast();
}
