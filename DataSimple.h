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
		// SimpleData abstracts POD and language-interoperable types
		//

		template <class T>
		class SimpleData : public ScalarData
		{
		public:
			virtual bool Copy( DataInstance src, DataInstance dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataInstance a, DataInstance b ) HELIUM_OVERRIDE;

			virtual void Serialize( DataInstance i, Stream& stream ) HELIUM_OVERRIDE;
			virtual void Deserialize( DataInstance i, Stream& stream, bool raiseChanged ) HELIUM_OVERRIDE;

			virtual void Serialize( DataInstance i, String& string) HELIUM_OVERRIDE;
			virtual void Deserialize( DataInstance i, const String& string, bool raiseChanged ) HELIUM_OVERRIDE;

			virtual void Accept( DataInstance i, Visitor& visitor ) HELIUM_OVERRIDE;

		private:
			inline SimpleDataType GetDataType();
		};

		template< class T >
		Data* AllocateData( const T& t )
		{
			return new SimpleData<T>;
		}
	}
}

#include "Reflect/DataSimple.inl"