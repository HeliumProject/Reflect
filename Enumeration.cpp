#include "ReflectPch.h"
#include "Enumeration.h"

#include "Foundation/Log.h"
#include "Foundation/String.h"

using namespace Helium;
using namespace Helium::Reflect;

#pragma TODO("Ditch STL in favor of Foundation containers")
#pragma TODO("Switch to uint64_t for wider bitfield support")
#pragma TODO("Make AddElement return an EnumerationElement for easier meta-data addition")

EnumerationElement::EnumerationElement()
	: m_Value( 0x0 )
{

}

EnumerationElement::EnumerationElement( uint32_t value, const tstring& name, const tstring& helpText )
	: m_Value( value )
	, m_Name( name )
	, m_HelpText( helpText )
{

}

Enumeration::Enumeration()
	: m_IsBitfield( true )
{

}

Enumeration::~Enumeration()
{

}

void Enumeration::Register() const
{
	Type::Register();

	uint32_t computedSize = 0;
	DynamicArray< EnumerationElement >::ConstIterator itr = m_Elements.Begin();
	DynamicArray< EnumerationElement >::ConstIterator end = m_Elements.End();
	for ( ; itr != end; ++itr )
	{
		Log::Debug( TXT( "  Value: %8d, Name: %s\n" ), itr->m_Value, itr->m_Name.c_str() );
	}
}

void Enumeration::Unregister() const
{
	Type::Unregister();
}

void Enumeration::AddElement( uint32_t value, const tstring& name, const tstring& helpText )
{
	EnumerationElement element ( value, name, helpText );

	m_Elements.Add( element );

	// our enumeration is treated as a bitfield if every value is a exactly a power-of-two
	if ( m_IsBitfield && ( ( value & ( value - 1 ) ) != 0 ) )
	{
		m_IsBitfield = false;
	}
}

bool Enumeration::IsValid(uint32_t value) const
{
	DynamicArray< EnumerationElement >::ConstIterator itr = m_Elements.Begin();
	DynamicArray< EnumerationElement >::ConstIterator end = m_Elements.End();
	for ( ; itr != end; ++itr )
	{
		if ( itr->m_Value == value )
		{
			return true;
		}
	}

	return false;
}

bool Enumeration::GetValue(const tstring& str, uint32_t& value) const
{
	if ( m_IsBitfield )
	{
		std::vector< tstring > strs;

		const tchar_t *string = str.c_str();

		while ( const tchar_t *stringFromPipe = FindCharacter( string, '|' ) )
		{
			tstring str(string, stringFromPipe - string);
			strs.push_back(str);
			string = stringFromPipe + 1;
		}
		
		strs.push_back( string );

		return GetBitfieldValue( strs, value );
	}
	else
	{
		return GetSingleValue( str, value );
	}
}

bool Enumeration::GetString(const uint32_t value, tstring& str) const
{
	if ( m_IsBitfield )
	{
		bool first = true;

		std::vector< tstring > strs;
		if (!GetBitfieldStrings(value, strs))
		{
			return false;
		}

		if ( strs.empty() )
		{
			return true;
		}

		// search the map
		std::vector< tstring >::const_iterator itr = strs.begin();
		std::vector< tstring >::const_iterator end = strs.end();
		for ( ; itr != end; ++itr )
		{
			if ( !first )
			{
				str += TXT("|");
			}

			first = false;

			str += *itr;
		}

		return !first;
	}
	else
	{
		DynamicArray< EnumerationElement >::ConstIterator itr = m_Elements.Begin();
		DynamicArray< EnumerationElement >::ConstIterator end = m_Elements.End();
		for ( ; itr != end; ++itr )
		{
			if ( itr->m_Value == value )
			{
				str = itr->m_Name;
				return true;
			}
		}

		return false;
	}
}

bool Enumeration::GetSingleValue(const tstring& str, uint32_t& value) const
{
	DynamicArray< EnumerationElement >::ConstIterator itr = m_Elements.Begin();
	DynamicArray< EnumerationElement >::ConstIterator end = m_Elements.End();
	for ( ; itr != end; ++itr )
	{
		if ( itr->m_Name == str )
		{
			value = itr->m_Value;
			return true;
		}
	}

	return false;
}

bool Enumeration::GetBitfieldValue(const std::vector< tstring >& strs, uint32_t& value) const
{
	value = 0;

	HELIUM_ASSERT( m_IsBitfield );
	if ( m_IsBitfield )
	{
		std::vector< tstring >::const_iterator itr = strs.begin();
		std::vector< tstring >::const_iterator end = strs.end();
		for ( ; itr != end; ++itr )
		{
			uint32_t flags;
			if (GetSingleValue(*itr, flags))
			{
				// set the bitfield value
				SetFlags( value, static_cast<uint32_t>(flags) );
			}
		}
	}

	return value == 0 || !strs.empty();
}

bool Enumeration::GetBitfieldStrings(const uint32_t value, std::vector< tstring >& strs) const
{
	HELIUM_ASSERT( m_IsBitfield );
	if ( m_IsBitfield )
	{
		// search the map
		DynamicArray< EnumerationElement >::ConstIterator itr = m_Elements.Begin();
		DynamicArray< EnumerationElement >::ConstIterator end = m_Elements.End();
		for ( ; itr != end; ++itr )
		{
			if ( IsFlagSet( value, itr->m_Value ) )
			{
				strs.push_back( itr->m_Name );
			}
		}
	}

	return value == 0 || !strs.empty();
}
