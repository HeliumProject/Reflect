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
			inline SimpleData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual bool Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Data* AllocateData( uint8_t,  uint8_t )    { return new SimpleData< uint8_t >; }
		inline Data* AllocateData( uint16_t, uint16_t )   { return new SimpleData< uint16_t >; }
		inline Data* AllocateData( uint32_t, uint32_t )   { return new SimpleData< uint32_t >; }
		inline Data* AllocateData( uint64_t, uint64_t )   { return new SimpleData< uint64_t >; }
		inline Data* AllocateData( int8_t,  int8_t )      { return new SimpleData< int8_t >; }
		inline Data* AllocateData( int16_t, int16_t )     { return new SimpleData< int16_t >; }
		inline Data* AllocateData( int32_t, int32_t )     { return new SimpleData< int32_t >; }
		inline Data* AllocateData( int64_t, int64_t )     { return new SimpleData< int64_t >; }
		inline Data* AllocateData( float32_t, float32_t ) { return new SimpleData< float32_t >; }
		inline Data* AllocateData( float64_t, float64_t ) { return new SimpleData< float64_t >; }
	}
}

#include "Reflect/DataSimple.inl"