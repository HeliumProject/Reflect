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
			REFLECT_META_DERIVED( MetaIds::MetaClass, MetaClass, MetaStruct );

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
			StrongPtr< Object >     m_Default;  // the template for this class (by default, the default instance)
		};

		typedef Helium::SmartPtr< MetaClass > ClassPtr;
		typedef Helium::SmartPtr< const MetaClass > ConstClassPtr;

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

#include "Reflect/MetaClass.inl"
