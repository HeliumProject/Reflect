#pragma once

#include "Platform/Encoding.h"

#include "Reflect/Structure.h"
#include "Reflect/Enumeration.h"
#include "Reflect/Registry.h"

namespace Helium
{
	namespace Reflect
	{
		// function type for creating object instances
		typedef Object* (*CreateObjectFunc)();

		//
		// Class (C++ `struct` or `class`)
		//

		class HELIUM_REFLECT_API Class : public Structure
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::Class, Class, Structure );

		protected:
			Class();
			~Class();

		public:
			// protect external allocation to keep inlined code in this dll
			static Class* Create();

			template< class ClassT >
			static void Create( Class const*& pointer, const tchar_t* name, const tchar_t* baseName, CreateObjectFunc creator = NULL );

		public:
			CreateObjectFunc        m_Creator;  // factory function for creating instances of this class
			StrongPtr< Object >     m_Default;  // the template for this class (by default, the default instance)
		};

		typedef Helium::SmartPtr< Class > ClassPtr;
		typedef Helium::SmartPtr< const Class > ConstClassPtr;

		// Object, the most base class needs explicit implementation
		template<>
		void Class::Create< Object >( Class const*& pointer, const tchar_t* name, const tchar_t* baseName, CreateObjectFunc creator );

		template< class ClassT, class BaseT >
		class ObjectRegistrar : public TypeRegistrar
		{
		public:
			ObjectRegistrar(const tchar_t* name);
			~ObjectRegistrar();

			virtual void Register();
			virtual void Unregister();
		};

		template< class ClassT >
		class ObjectRegistrar< ClassT, void > : public TypeRegistrar
		{
		public:
			ObjectRegistrar(const tchar_t* name);
			~ObjectRegistrar();

			virtual void Register();
			virtual void Unregister();
		};
	}
}

#include "Reflect/Class.inl"
