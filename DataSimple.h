#pragma once

#include <sstream>
#include <type_traits>

#include "Foundation/TUID.h"
#include "Foundation/Name.h"

#include "Reflect/Data.h"

namespace Helium
{
	namespace Reflect
	{
		//
		// SimpleData abstracts POD and language-interoperable binary-serializable types
		//

		template <class T>
		class SimpleData : public ScalarData
		{
		public:
			virtual bool Copy( DataInstance src, DataInstance dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataInstance a, DataInstance b ) HELIUM_OVERRIDE;
			virtual void Accept( DataInstance i, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataInstance i, String& string, ObjectIdentifier& identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataInstance i, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		Data* AllocateData( const T& t )
		{
			return new SimpleData<T>;
		}
	}
}

#include "Reflect/DataSimple.inl"