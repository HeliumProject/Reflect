#include "Precompile.h"
#include "MetaEnum.h"

#include "Foundation/Log.h"
#include "Foundation/String.h"

using namespace Helium;
using namespace Helium::Reflect;

// TODO: Ditch STL in favor of Foundation containers
// TODO: Switch to uint64_t for wider bitfield support
// TODO: Make AddElement return an MetaEnum::Element for easier meta-data addition

MetaEnum::Element::Element()
	: m_Value( 0x0 )
{

}

MetaEnum::Element::Element( uint32_t value, const std::string& name, const std::string& helpText )
	: m_Value( value )
	, m_Name( name )
	, m_HelpText( helpText )
{

}

MetaEnum::MetaEnum()
	: m_IsBitfield( true )
{

}

MetaEnum::~MetaEnum()
{

}

void MetaEnum::Register() const
{
	MetaType::Register();

	uint32_t computedSize = 0;
	DynamicArray< MetaEnum::Element >::ConstIterator itr = m_Elements.Begin();
	DynamicArray< MetaEnum::Element >::ConstIterator end = m_Elements.End();
	for ( ; itr != end; ++itr )
	{
		Log::Debug( "  Value: %8d, Name: %s\n", itr->m_Value, itr->m_Name.c_str() );
	}
}

void MetaEnum::Unregister() const
{
	MetaType::Unregister();
}

void MetaEnum::AddElement( uint32_t value, const std::string& name, const std::string& helpText )
{
	MetaEnum::Element element ( value, name, helpText );

	m_Elements.Add( element );

	// our enumeration is treated as a bitfield if every value is a exactly a power-of-two
	if ( m_IsBitfield && ( ( value & ( value - 1 ) ) != 0 ) )
	{
		m_IsBitfield = false;
	}
}

bool MetaEnum::IsValid(uint32_t value) const
{
	DynamicArray< MetaEnum::Element >::ConstIterator itr = m_Elements.Begin();
	DynamicArray< MetaEnum::Element >::ConstIterator end = m_Elements.End();
	for ( ; itr != end; ++itr )
	{
		if ( itr->m_Value == value )
		{
			return true;
		}
	}

	return false;
}

bool MetaEnum::GetValue(const std::string& str, uint32_t& value) const
{
	if ( m_IsBitfield )
	{
		std::vector< std::string > strs;

		const char *string = str.c_str();

		while ( const char *stringFromPipe = FindCharacter( string, '|' ) )
		{
			std::string str(string, stringFromPipe - string);
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

bool MetaEnum::GetString(const uint32_t value, std::string& str) const
{
	if ( m_IsBitfield )
	{
		bool first = true;

		std::vector< std::string > strs;
		if (!GetBitfieldStrings(value, strs))
		{
			return false;
		}

		if ( strs.empty() )
		{
			return true;
		}

		// search the map
		std::vector< std::string >::const_iterator itr = strs.begin();
		std::vector< std::string >::const_iterator end = strs.end();
		for ( ; itr != end; ++itr )
		{
			if ( !first )
			{
				str += "|";
			}

			first = false;

			str += *itr;
		}

		return !first;
	}
	else
	{
		DynamicArray< MetaEnum::Element >::ConstIterator itr = m_Elements.Begin();
		DynamicArray< MetaEnum::Element >::ConstIterator end = m_Elements.End();
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

bool MetaEnum::GetSingleValue(const std::string& str, uint32_t& value) const
{
	DynamicArray< MetaEnum::Element >::ConstIterator itr = m_Elements.Begin();
	DynamicArray< MetaEnum::Element >::ConstIterator end = m_Elements.End();
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

bool MetaEnum::GetBitfieldValue(const std::vector< std::string >& strs, uint32_t& value) const
{
	value = 0;

	HELIUM_ASSERT( m_IsBitfield );
	if ( m_IsBitfield )
	{
		std::vector< std::string >::const_iterator itr = strs.begin();
		std::vector< std::string >::const_iterator end = strs.end();
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

bool MetaEnum::GetBitfieldStrings(const uint32_t value, std::vector< std::string >& strs) const
{
	HELIUM_ASSERT( m_IsBitfield );
	if ( m_IsBitfield )
	{
		// search the map
		DynamicArray< MetaEnum::Element >::ConstIterator itr = m_Elements.Begin();
		DynamicArray< MetaEnum::Element >::ConstIterator end = m_Elements.End();
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
