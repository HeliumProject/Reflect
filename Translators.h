#pragma once

#include <sstream>
#include <type_traits>

#include "Foundation/TUID.h"
#include "Foundation/Name.h"

#include "Reflect/Translator.h"

namespace Helium
{
	namespace Reflect
	{
		template <class T>
		class SimpleScalarTranslator : public ScalarTranslator
		{
		public:
			inline SimpleScalarTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Accept( Pointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Translator* AllocateTranslator( bool,  bool )          { return new SimpleScalarTranslator< bool >; }
		inline Translator* AllocateTranslator( uint8_t,  uint8_t )    { return new SimpleScalarTranslator< uint8_t >; }
		inline Translator* AllocateTranslator( uint16_t, uint16_t )   { return new SimpleScalarTranslator< uint16_t >; }
		inline Translator* AllocateTranslator( uint32_t, uint32_t )   { return new SimpleScalarTranslator< uint32_t >; }
		inline Translator* AllocateTranslator( uint64_t, uint64_t )   { return new SimpleScalarTranslator< uint64_t >; }
		inline Translator* AllocateTranslator( int8_t,  int8_t )      { return new SimpleScalarTranslator< int8_t >; }
		inline Translator* AllocateTranslator( int16_t, int16_t )     { return new SimpleScalarTranslator< int16_t >; }
		inline Translator* AllocateTranslator( int32_t, int32_t )     { return new SimpleScalarTranslator< int32_t >; }
		inline Translator* AllocateTranslator( int64_t, int64_t )     { return new SimpleScalarTranslator< int64_t >; }
		inline Translator* AllocateTranslator( float32_t, float32_t ) { return new SimpleScalarTranslator< float32_t >; }
		inline Translator* AllocateTranslator( float64_t, float64_t ) { return new SimpleScalarTranslator< float64_t >; }

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class SimpleStructureTranslator : public StructureTranslator
		{
		public:
			inline SimpleStructureTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Accept( Pointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual const Structure* GetStructure() const HELIUM_OVERRIDE;
		};

		template< class T >
		Translator* AllocateTranslator( const StructureBase&, T )
		{
			return new SimpleStructureTranslator< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class PointerTranslator : public ScalarTranslator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::PointerTranslator, PointerTranslator, ScalarTranslator );

			inline PointerTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Accept( Pointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		Translator* AllocateTranslator( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return new PointerTranslator< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class EnumerationTranslator : public ScalarTranslator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::EnumerationTranslator, EnumerationTranslator, ScalarTranslator );

			inline EnumerationTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Accept( Pointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		Translator* AllocateTranslator( const EnumerationBase&, T )
		{
			return new EnumerationTranslator< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		class TypeTranslator : public ScalarTranslator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::TypeTranslator, TypeTranslator, ScalarTranslator );

			TypeTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Accept( Pointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Translator* AllocateTranslator( const Type*, const Type* )
		{
			return new TypeTranslator;
		}
	}
}

#include "Reflect/Translators.inl"