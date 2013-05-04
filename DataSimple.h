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
		template <class T>
		class SimpleScalarData : public ScalarData
		{
		public:
			inline SimpleScalarData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Data* AllocateData( uint8_t,  uint8_t )    { return new SimpleScalarData< uint8_t >; }
		inline Data* AllocateData( uint16_t, uint16_t )   { return new SimpleScalarData< uint16_t >; }
		inline Data* AllocateData( uint32_t, uint32_t )   { return new SimpleScalarData< uint32_t >; }
		inline Data* AllocateData( uint64_t, uint64_t )   { return new SimpleScalarData< uint64_t >; }
		inline Data* AllocateData( int8_t,  int8_t )      { return new SimpleScalarData< int8_t >; }
		inline Data* AllocateData( int16_t, int16_t )     { return new SimpleScalarData< int16_t >; }
		inline Data* AllocateData( int32_t, int32_t )     { return new SimpleScalarData< int32_t >; }
		inline Data* AllocateData( int64_t, int64_t )     { return new SimpleScalarData< int64_t >; }
		inline Data* AllocateData( float32_t, float32_t ) { return new SimpleScalarData< float32_t >; }
		inline Data* AllocateData( float64_t, float64_t ) { return new SimpleScalarData< float64_t >; }

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class SimpleStructureData : public StructureData
		{
		public:
			inline SimpleStructureData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual const Structure* GetStructure() const HELIUM_OVERRIDE;
		};

		template< class T >
		Data* AllocateData( const StructureBase&, T )
		{
			return new SimpleStructureData< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class PointerData : public ScalarData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::PointerData, PointerData, ScalarData );

			inline PointerData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		Data* AllocateData( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return new PointerData< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class EnumerationData : public ScalarData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::EnumerationData, EnumerationData, ScalarData );

			inline EnumerationData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		Data* AllocateData( const EnumerationBase&, T )
		{
			return new EnumerationData< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		class TypeData : public ScalarData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::TypeData, TypeData, ScalarData );

			TypeData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Data* AllocateData( const Type*, const Type* )
		{
			return new TypeData;
		}
	}
}

#include "Reflect/DataSimple.inl"