#pragma once

#include <sstream>

#include "Platform/Types.h"
#include "Platform/Utility.h"

#include "Foundation/TUID.h"
#include "Foundation/Name.h"

#include "Reflect/Translator.h"
#include "Reflect/Structure.h"
#include "Reflect/Enumeration.h"

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
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template<> Helium::Reflect::SimpleScalarTranslator< bool >::SimpleScalarTranslator()      : ScalarTranslator( 1, ScalarTypes::Boolean ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< uint8_t >::SimpleScalarTranslator()   : ScalarTranslator( 1, ScalarTypes::Unsigned8 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< uint16_t >::SimpleScalarTranslator()  : ScalarTranslator( 2, ScalarTypes::Unsigned16 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< uint32_t >::SimpleScalarTranslator()  : ScalarTranslator( 4, ScalarTypes::Unsigned32 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< uint64_t >::SimpleScalarTranslator()  : ScalarTranslator( 8, ScalarTypes::Unsigned64 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< int8_t >::SimpleScalarTranslator()    : ScalarTranslator( 1, ScalarTypes::Signed8 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< int16_t >::SimpleScalarTranslator()   : ScalarTranslator( 2, ScalarTypes::Signed16 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< int32_t >::SimpleScalarTranslator()   : ScalarTranslator( 4, ScalarTypes::Signed32 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< int64_t >::SimpleScalarTranslator()   : ScalarTranslator( 8, ScalarTypes::Signed64 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< float32_t >::SimpleScalarTranslator() : ScalarTranslator( 4, ScalarTypes::Float32 ) {}
		template<> Helium::Reflect::SimpleScalarTranslator< float64_t >::SimpleScalarTranslator() : ScalarTranslator( 8, ScalarTypes::Float64 ) {}

		inline const Type* DeduceKeyType( bool, bool )                { return NULL; }
		inline const Type* DeduceValueType( bool, bool )              { return NULL; }
		inline Translator* AllocateTranslator( bool,  bool )          { return new SimpleScalarTranslator< bool >; }

		inline const Type* DeduceKeyType( uint8_t, uint8_t )          { return NULL; }
		inline const Type* DeduceValueType( uint8_t, uint8_t )        { return NULL; }
		inline Translator* AllocateTranslator( uint8_t,  uint8_t )    { return new SimpleScalarTranslator< uint8_t >; }

		inline const Type* DeduceKeyType( uint16_t, uint16_t )        { return NULL; }
		inline const Type* DeduceValueType( uint16_t, uint16_t )      { return NULL; }
		inline Translator* AllocateTranslator( uint16_t, uint16_t )   { return new SimpleScalarTranslator< uint16_t >; }

		inline const Type* DeduceKeyType( uint32_t, uint32_t )        { return NULL; }
		inline const Type* DeduceValueType( uint32_t, uint32_t )      { return NULL; }
		inline Translator* AllocateTranslator( uint32_t, uint32_t )   { return new SimpleScalarTranslator< uint32_t >; }

		inline const Type* DeduceKeyType( uint64_t, uint64_t )        { return NULL; }
		inline const Type* DeduceValueType( uint64_t, uint64_t )      { return NULL; }
		inline Translator* AllocateTranslator( uint64_t, uint64_t )   { return new SimpleScalarTranslator< uint64_t >; }

		inline const Type* DeduceKeyType( int8_t, int8_t )            { return NULL; }
		inline const Type* DeduceValueType( int8_t, int8_t )          { return NULL; }
		inline Translator* AllocateTranslator( int8_t,  int8_t )      { return new SimpleScalarTranslator< int8_t >; }

		inline const Type* DeduceKeyType( int16_t, int16_t )          { return NULL; }
		inline const Type* DeduceValueType( int16_t, int16_t )        { return NULL; }
		inline Translator* AllocateTranslator( int16_t, int16_t )     { return new SimpleScalarTranslator< int16_t >; }

		inline const Type* DeduceKeyType( int32_t, int32_t )          { return NULL; }
		inline const Type* DeduceValueType( int32_t, int32_t )        { return NULL; }
		inline Translator* AllocateTranslator( int32_t, int32_t )     { return new SimpleScalarTranslator< int32_t >; }

		inline const Type* DeduceKeyType( int64_t, int64_t )          { return NULL; }
		inline const Type* DeduceValueType( int64_t, int64_t )        { return NULL; }
		inline Translator* AllocateTranslator( int64_t, int64_t )     { return new SimpleScalarTranslator< int64_t >; }

		inline const Type* DeduceKeyType( float32_t, float32_t )      { return NULL; }
		inline const Type* DeduceValueType( float32_t, float32_t )    { return NULL; }
		inline Translator* AllocateTranslator( float32_t, float32_t ) { return new SimpleScalarTranslator< float32_t >; }

		inline const Type* DeduceKeyType( float64_t, float64_t )      { return NULL; }
		inline const Type* DeduceValueType( float64_t, float64_t )    { return NULL; }
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
			virtual const Structure* GetStructure() const HELIUM_OVERRIDE;
		};

		template< class T >
		const Type* DeduceKeyType( const StructureBase&, T )
		{
			return NULL;
		}
		template< class T >
		const Type* DeduceValueType( const StructureBase&, T )
		{
			return Reflect::GetStructure< T >();
		}
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
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		const Type* DeduceKeyType( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return NULL;
		}
		template< class T >
		const Type* DeduceValueType( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return Reflect::GetClass< T >();
		}
		template< class T >
		Translator* AllocateTranslator( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return new PointerTranslator< T >;
		}

		void Identify( ObjectIdentifier* identifier, Pointer pointer, Name& name );
		void Resolve( ObjectResolver* resolver, Name name, Pointer pointer );

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
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		const Type* DeduceKeyType( const EnumerationBase&, T )
		{
			return NULL;
		}
		template< class T >
		const Type* DeduceValueType( const EnumerationBase&, T )
		{
			return Reflect::GetEnumeration< T >();
		}
		template< class T >
		Translator* AllocateTranslator( const EnumerationBase&, T )
		{
			return new EnumerationTranslator< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		class HELIUM_REFLECT_API TypeTranslator : public ScalarTranslator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::TypeTranslator, TypeTranslator, ScalarTranslator );

			TypeTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline const Type* DeduceKeyType( const Type*, const Type* )
		{
			return NULL;
		}
		inline const Type* DeduceValueType( const Type*, const Type* )
		{
			return NULL;
		}
		inline Translator* AllocateTranslator( const Type*, const Type* )
		{
			return new TypeTranslator;
		}
	}
}

#include "Reflect/Translators.inl"