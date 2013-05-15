template<class T>
void Helium::Reflect::PropertyCollection::SetProperty( const tstring& key, const T& value ) const
{
	tostringstream str;
	str << value;

	if ( !str.fail() )
	{
		SetProperty<tstring>( key, str.str() );
	}
}

namespace Helium
{
	namespace Reflect
	{
		template<>
		inline void PropertyCollection::SetProperty( const tstring& key, const tstring& value ) const
		{
			m_Properties[key] = value;
		}
	}
}

template<class T>
bool Helium::Reflect::PropertyCollection::GetProperty( const tstring& key, T& value ) const
{
	tstring strValue;
	bool result = GetProperty<tstring>( key, strValue );

	if ( result )
	{
		tistringstream str( strValue );
		str >> value;
		return !str.fail();
	}

	return false;
}

namespace Helium
{
	namespace Reflect
	{
		template<>
		inline bool PropertyCollection::GetProperty( const tstring& key, tstring& value ) const
		{
			std::map< tstring, tstring >::const_iterator found = m_Properties.find( key ); 
			if ( found != m_Properties.end() )
			{
				value = found->second;
				return true;
			}

			return false;
		}
	}
}

inline const tstring& Helium::Reflect::PropertyCollection::GetProperty( const tstring& key ) const
{
	std::map< tstring, tstring >::const_iterator found = m_Properties.find( key );
	if ( found != m_Properties.end() )
	{
		return found->second;
	}

	static tstring empty;
	return empty;
}
