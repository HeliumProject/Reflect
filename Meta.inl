template<class T>
void Helium::Reflect::PropertyCollection::SetProperty( const std::string& key, const T& value ) const
{
	std::ostringstream str;
	str << value;

	if ( !str.fail() )
	{
		SetProperty<std::string>( key, str.str() );
	}
}

namespace Helium
{
	namespace Reflect
	{
		template<>
		inline void PropertyCollection::SetProperty( const std::string& key, const std::string& value ) const
		{
			m_Properties[key] = value;
		}
	}
}

template<class T>
bool Helium::Reflect::PropertyCollection::GetProperty( const std::string& key, T& value ) const
{
	std::string strValue;
	bool result = GetProperty<std::string>( key, strValue );

	if ( result )
	{
		std::istringstream str( strValue );
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
		inline bool PropertyCollection::GetProperty( const std::string& key, std::string& value ) const
		{
			std::map< std::string, std::string >::const_iterator found = m_Properties.find( key ); 
			if ( found != m_Properties.end() )
			{
				value = found->second;
				return true;
			}

			return false;
		}
	}
}

inline std::string Helium::Reflect::PropertyCollection::GetProperty( const std::string& key ) const
{
	std::map< std::string, std::string >::const_iterator found = m_Properties.find( key );
	if ( found != m_Properties.end() )
	{
		return found->second;
	}

	return std::string ();
}
