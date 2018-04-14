#pragma once

#include "Platform/Encoding.h"

#include "Reflect/MetaStruct.h"
#include "Reflect/MetaEnum.h"
#include "Reflect/Registry.h"

namespace Helium
{
	namespace Reflect
	{
		// function type for creating object instances
		typedef Object* (*CreateObjectFunc)();

		//
		// MetaClass (C++ `class`)
		//

		class HELIUM_REFLECT_API MetaClass : public MetaStruct
		{
		public:
			HELIUM_META_DERIVED( MetaIds::MetaClass, MetaClass, MetaStruct );

		protected:
			MetaClass();
			~MetaClass();

		public:
			// protect external allocation to keep inlined code in this dll
			static MetaClass* Create();

			template< class ClassT >
			static void Create( MetaClass const*& pointer, const char* name, const char* baseName, CreateObjectFunc creator = NULL );

		public:
			CreateObjectFunc        m_Creator;  // factory function for creating instances of this class
			StrongPtr< Object >     m_Default;  // a default instance of an object of this class
		};

		// Object, the most base class needs explicit implementation
		template<>
		void MetaClass::Create< Object >( MetaClass const*& pointer, const char* name, const char* baseName, CreateObjectFunc creator );

		template< class ClassT, class BaseT >
		class MetaClassRegistrar : public MetaTypeRegistrar
		{
		public:
			MetaClassRegistrar(const char* name);
			~MetaClassRegistrar();

			virtual void Register();
			virtual void Unregister();
		};

		template< class ClassT >
		class MetaClassRegistrar< ClassT, void > : public MetaTypeRegistrar
		{
		public:
			MetaClassRegistrar(const char* name);
			~MetaClassRegistrar();

			virtual void Register();
			virtual void Unregister();
		};
	}
}

// declares creator for constructable types
#define _REFLECT_DECLARE_CREATOR( OBJECT ) \
public: \
static Helium::Reflect::Object* CreateObject() { return new OBJECT; }

// declares type checking functions
#define _REFLECT_DECLARE_CLASS( OBJECT, BASE ) \
public: \
typedef BASE Base; \
typedef OBJECT This; \
virtual const Helium::Reflect::MetaClass* GetMetaClass() const override; \
static const Helium::Reflect::MetaClass* CreateMetaClass(); \
static const Helium::Reflect::MetaClass* s_MetaClass;

#define _REFLECT_DECLARE_CLASS_REGISTRAR( OBJECT, BASE ) \
static Helium::Reflect::MetaClassRegistrar< OBJECT, BASE > s_Registrar;

// defines the static type info vars
#define _REFLECT_DEFINE_CLASS( OBJECT, CREATOR ) \
const Helium::Reflect::MetaClass* OBJECT::GetMetaClass() const \
{ \
	return s_MetaClass; \
} \
\
const Helium::Reflect::MetaClass* OBJECT::CreateMetaClass() \
{ \
	HELIUM_ASSERT( s_MetaClass == NULL ); \
	HELIUM_ASSERT( OBJECT::Base::s_MetaClass != NULL ); \
	Helium::Reflect::MetaClass::Create< OBJECT >( s_MetaClass, #OBJECT, OBJECT::Base::s_MetaClass->m_Name, CREATOR); \
	return s_MetaClass; \
} \
const Helium::Reflect::MetaClass* OBJECT::s_MetaClass = NULL;

#define _REFLECT_DEFINE_CLASS_REGISTRAR( OBJECT, CREATOR ) \
Helium::Reflect::MetaClassRegistrar< OBJECT, OBJECT::Base > OBJECT::s_Registrar( #OBJECT );

// declares an abstract object (an object that either A: cannot be instantiated or B: is never actually serialized)
#define HELIUM_DECLARE_ABSTRACT_NO_REGISTRAR( OBJECT, BASE ) \
	_REFLECT_DECLARE_CLASS( OBJECT, BASE )

#define HELIUM_DECLARE_ABSTRACT( OBJECT, BASE ) \
	HELIUM_DECLARE_ABSTRACT_NO_REGISTRAR( OBJECT, BASE ) \
	_REFLECT_DECLARE_CLASS_REGISTRAR( OBJECT, BASE )

// defines the abstract object class
#define HELIUM_DEFINE_ABSTRACT_NO_REGISTRAR( OBJECT ) \
	_REFLECT_DEFINE_CLASS( OBJECT, NULL )

// defines the abstract object class
#define HELIUM_DEFINE_ABSTRACT( OBJECT ) \
	HELIUM_DEFINE_ABSTRACT_NO_REGISTRAR( OBJECT ) \
	_REFLECT_DEFINE_CLASS_REGISTRAR( OBJECT, NULL )

// declares a concrete object with creator
#define HELIUM_DECLARE_CLASS_NO_REGISTRAR( OBJECT, BASE ) \
	_REFLECT_DECLARE_CLASS( OBJECT, BASE ) \
	_REFLECT_DECLARE_CREATOR( OBJECT )

// declares a concrete object with creator
#define HELIUM_DECLARE_CLASS( OBJECT, BASE ) \
	HELIUM_DECLARE_CLASS_NO_REGISTRAR( OBJECT, BASE ) \
	_REFLECT_DECLARE_CLASS_REGISTRAR( OBJECT, BASE )

// defines a concrete object
#define HELIUM_DEFINE_CLASS_NO_REGISTRAR( OBJECT ) \
	_REFLECT_DEFINE_CLASS( OBJECT, &OBJECT::CreateObject )

// defines a concrete object
#define HELIUM_DEFINE_CLASS( OBJECT ) \
	HELIUM_DEFINE_CLASS_NO_REGISTRAR( OBJECT ) \
	_REFLECT_DEFINE_CLASS_REGISTRAR( OBJECT, &OBJECT::CreateObject )

#include "Reflect/MetaClass.inl"
