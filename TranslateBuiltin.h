#pragma once

#include <sstream>

#include "Platform/Types.h"
#include "Platform/Utility.h"

#include "Foundation/TUID.h"
#include "Foundation/Name.h"

#include "Reflect/Translator.h"
#include "Reflect/MetaStruct.h"
#include "Reflect/MetaEnum.h"

namespace Helium
{
	namespace Reflect
	{
		template <class T>
		class SimpleScalarTranslator : public ScalarTranslator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::SimpleTranslator, SimpleScalarTranslator, ScalarTranslator );

			inline SimpleScalarTranslator();
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		template<> inline SimpleScalarTranslator< bool >::SimpleScalarTranslator()      : ScalarTranslator( 1, ScalarTypes::Boolean ) {}
		template<> inline SimpleScalarTranslator< uint8_t >::SimpleScalarTranslator()   : ScalarTranslator( 1, ScalarTypes::Unsigned8 ) {}
		template<> inline SimpleScalarTranslator< uint16_t >::SimpleScalarTranslator()  : ScalarTranslator( 2, ScalarTypes::Unsigned16 ) {}
		template<> inline SimpleScalarTranslator< uint32_t >::SimpleScalarTranslator()  : ScalarTranslator( 4, ScalarTypes::Unsigned32 ) {}
		template<> inline SimpleScalarTranslator< uint64_t >::SimpleScalarTranslator()  : ScalarTranslator( 8, ScalarTypes::Unsigned64 ) {}
		template<> inline SimpleScalarTranslator< int8_t >::SimpleScalarTranslator()    : ScalarTranslator( 1, ScalarTypes::Signed8 ) {}
		template<> inline SimpleScalarTranslator< int16_t >::SimpleScalarTranslator()   : ScalarTranslator( 2, ScalarTypes::Signed16 ) {}
		template<> inline SimpleScalarTranslator< int32_t >::SimpleScalarTranslator()   : ScalarTranslator( 4, ScalarTypes::Signed32 ) {}
		template<> inline SimpleScalarTranslator< int64_t >::SimpleScalarTranslator()   : ScalarTranslator( 8, ScalarTypes::Signed64 ) {}
		template<> inline SimpleScalarTranslator< float32_t >::SimpleScalarTranslator() : ScalarTranslator( 4, ScalarTypes::Float32 ) {}
		template<> inline SimpleScalarTranslator< float64_t >::SimpleScalarTranslator() : ScalarTranslator( 8, ScalarTypes::Float64 ) {}

		inline const MetaType* DeduceKeyType( bool, bool )                { return NULL; }
		inline const MetaType* DeduceValueType( bool, bool )              { return NULL; }
		inline Translator* AllocateTranslator( bool,  bool )          { return new SimpleScalarTranslator< bool >; }

		inline const MetaType* DeduceKeyType( uint8_t, uint8_t )          { return NULL; }
		inline const MetaType* DeduceValueType( uint8_t, uint8_t )        { return NULL; }
		inline Translator* AllocateTranslator( uint8_t,  uint8_t )    { return new SimpleScalarTranslator< uint8_t >; }

		inline const MetaType* DeduceKeyType( uint16_t, uint16_t )        { return NULL; }
		inline const MetaType* DeduceValueType( uint16_t, uint16_t )      { return NULL; }
		inline Translator* AllocateTranslator( uint16_t, uint16_t )   { return new SimpleScalarTranslator< uint16_t >; }

		inline const MetaType* DeduceKeyType( uint32_t, uint32_t )        { return NULL; }
		inline const MetaType* DeduceValueType( uint32_t, uint32_t )      { return NULL; }
		inline Translator* AllocateTranslator( uint32_t, uint32_t )   { return new SimpleScalarTranslator< uint32_t >; }

		inline const MetaType* DeduceKeyType( uint64_t, uint64_t )        { return NULL; }
		inline const MetaType* DeduceValueType( uint64_t, uint64_t )      { return NULL; }
		inline Translator* AllocateTranslator( uint64_t, uint64_t )   { return new SimpleScalarTranslator< uint64_t >; }

		inline const MetaType* DeduceKeyType( int8_t, int8_t )            { return NULL; }
		inline const MetaType* DeduceValueType( int8_t, int8_t )          { return NULL; }
		inline Translator* AllocateTranslator( int8_t,  int8_t )      { return new SimpleScalarTranslator< int8_t >; }

		inline const MetaType* DeduceKeyType( int16_t, int16_t )          { return NULL; }
		inline const MetaType* DeduceValueType( int16_t, int16_t )        { return NULL; }
		inline Translator* AllocateTranslator( int16_t, int16_t )     { return new SimpleScalarTranslator< int16_t >; }

		inline const MetaType* DeduceKeyType( int32_t, int32_t )          { return NULL; }
		inline const MetaType* DeduceValueType( int32_t, int32_t )        { return NULL; }
		inline Translator* AllocateTranslator( int32_t, int32_t )     { return new SimpleScalarTranslator< int32_t >; }

		inline const MetaType* DeduceKeyType( int64_t, int64_t )          { return NULL; }
		inline const MetaType* DeduceValueType( int64_t, int64_t )        { return NULL; }
		inline Translator* AllocateTranslator( int64_t, int64_t )     { return new SimpleScalarTranslator< int64_t >; }

		inline const MetaType* DeduceKeyType( float32_t, float32_t )      { return NULL; }
		inline const MetaType* DeduceValueType( float32_t, float32_t )    { return NULL; }
		inline Translator* AllocateTranslator( float32_t, float32_t ) { return new SimpleScalarTranslator< float32_t >; }

		inline const MetaType* DeduceKeyType( float64_t, float64_t )      { return NULL; }
		inline const MetaType* DeduceValueType( float64_t, float64_t )    { return NULL; }
		inline Translator* AllocateTranslator( float64_t, float64_t ) { return new SimpleScalarTranslator< float64_t >; }

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class SimpleStructureTranslator : public StructureTranslator
		{
		public:
			inline SimpleStructureTranslator();
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual const MetaStruct* GetMetaStruct() const override;
		};

		template< class T >
		const MetaType* DeduceKeyType( const Struct&, const T& )
		{
			return NULL;
		}
		template< class T >
		const MetaType* DeduceValueType( const Struct&, const T& )
		{
			return Reflect::GetMetaStruct< T >();
		}
		template< class T >
		Translator* AllocateTranslator( const Struct&, const T& )
		{
			return new SimpleStructureTranslator< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class PointerTranslator : public ScalarTranslator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::PointerTranslator, PointerTranslator, ScalarTranslator );

			inline PointerTranslator();
			virtual uint32_t GetDefaultFlags() override;
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		template< class T >
		const MetaType* DeduceKeyType( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return NULL;
		}
		template< class T >
		const MetaType* DeduceValueType( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return Reflect::GetMetaClass< T >();
		}
		template< class T >
		Translator* AllocateTranslator( const StrongPtr< T >&, const StrongPtr< T >& )
		{
			return new PointerTranslator< T >;
		}

		HELIUM_REFLECT_API bool Identify( ObjectIdentifier* identifier, Pointer pointer, Name* name );
		HELIUM_REFLECT_API void Resolve( ObjectResolver* resolver, Name name, Pointer pointer );

		//////////////////////////////////////////////////////////////////////////

		template< class T >
		class EnumerationTranslator : public ScalarTranslator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::EnumerationTranslator, EnumerationTranslator, ScalarTranslator );

			inline EnumerationTranslator();
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		template< class T >
		const MetaType* DeduceKeyType( const Enum&, const T& )
		{
			return NULL;
		}
		template< class T >
		const MetaType* DeduceValueType( const Enum&, const T& )
		{
			return Reflect::GetMetaEnum< T >();
		}
		template< class T >
		Translator* AllocateTranslator( const Enum&, const T& )
		{
			return new EnumerationTranslator< T >;
		}

		//////////////////////////////////////////////////////////////////////////

		class HELIUM_REFLECT_API TypeTranslator : public ScalarTranslator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::TypeTranslator, TypeTranslator, ScalarTranslator );

			TypeTranslator();
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		inline const MetaType* DeduceKeyType( const MetaType*, const MetaType* )
		{
			return NULL;
		}
		inline const MetaType* DeduceValueType( const MetaType*, const MetaType* )
		{
			return NULL;
		}
		inline Translator* AllocateTranslator( const MetaType*, const MetaType* )
		{
			return new TypeTranslator;
		}
	}
}

#include "Reflect/TranslateBuiltin.inl"