#pragma once

#include "Foundation/SmartPtr.h"

#include "Reflect/Composite.h"

namespace Helium
{
	namespace Reflect
	{
		// Empty struct just for type deduction purposes
		//  don't worry though, even though this class is non-zero in size on its own,
		//  your derived struct type can use the memory this takes due to C/C++ standard
		//  'Empty Base Optimization'
		struct HELIUM_REFLECT_API StructBase {};

		//
		// Structure (struct or class)
		//

		class HELIUM_REFLECT_API Structure : public Composite
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::Structure, Structure, Composite );

		protected:
			Structure();
			~Structure();

		public:
			// protect external allocation to keep inlined code in this dll
			static Structure* Create();

			template< class StructureT >
			static void Create( Structure const*& pointer, const tchar_t* name, const tchar_t* baseName )
			{
				Structure* type = Structure::Create();
				pointer = type;

				// populate reflection information
				Composite::Create< StructureT >( name, baseName, &StructureT::PopulateComposite, type );

				type->m_Default = new StructureT;
			}
		};

		template< class ClassT, class BaseT >
		class StructureRegistrar : public TypeRegistrar
		{
		public:
			StructureRegistrar(const tchar_t* name);
			~StructureRegistrar();

			virtual void Register();
			virtual void Unregister();
		};

		template< class ClassT >
		class StructureRegistrar< ClassT, void > : public TypeRegistrar
		{
		public:
			StructureRegistrar(const tchar_t* name);
			~StructureRegistrar();

			virtual void Register();
			virtual void Unregister();
		};
	}
}

// declares type checking functions
#define _REFLECT_DECLARE_BASE_STRUCTURE( STRUCTURE ) \
public: \
typedef STRUCTURE This; \
static const Helium::Reflect::Structure* CreateStructure(); \
static const Helium::Reflect::Structure* s_Structure; \
static Helium::Reflect::StructureRegistrar< STRUCTURE, void > s_Registrar;

#define _REFLECT_DECLARE_DERIVED_STRUCTURE( STRUCTURE, BASE ) \
public: \
typedef BASE Base; \
typedef STRUCTURE This; \
static const Helium::Reflect::Structure* CreateStructure(); \
static const Helium::Reflect::Structure* s_Structure; \
static Helium::Reflect::StructureRegistrar< STRUCTURE, BASE > s_Registrar;

// defines the static type info vars
#define _REFLECT_DEFINE_BASE_STRUCTURE( STRUCTURE ) \
const Helium::Reflect::Structure* STRUCTURE::CreateStructure() \
{ \
    HELIUM_ASSERT( s_Structure == NULL ); \
    Helium::Reflect::Structure::Create<STRUCTURE>( s_Structure, TXT( #STRUCTURE ), NULL ); \
    return s_Structure; \
} \
const Helium::Reflect::Structure* STRUCTURE::s_Structure = NULL; \
Helium::Reflect::StructureRegistrar< STRUCTURE, void > STRUCTURE::s_Registrar( TXT( #STRUCTURE ) );

#define _REFLECT_DEFINE_DERIVED_STRUCTURE( STRUCTURE ) \
const Helium::Reflect::Structure* STRUCTURE::CreateStructure() \
{ \
    HELIUM_ASSERT( s_Structure == NULL ); \
    HELIUM_ASSERT( STRUCTURE::Base::s_Structure != NULL ); \
    Helium::Reflect::Structure::Create<STRUCTURE>( s_Structure, TXT( #STRUCTURE ), STRUCTURE::Base::s_Structure->m_Name ); \
    return s_Structure; \
} \
const Helium::Reflect::Structure* STRUCTURE::s_Structure = NULL; \
Helium::Reflect::StructureRegistrar< STRUCTURE, BASE > STRUCTURE::s_Registrar( TXT( #STRUCTURE ) );

// declares a concrete object with creator
#define REFLECT_DECLARE_BASE_STRUCTURE( STRUCTURE ) \
    _REFLECT_DECLARE_BASE_STRUCTURE( STRUCTURE )

#define REFLECT_DECLARE_DERIVED_STRUCTURE( STRUCTURE, BASE ) \
    _REFLECT_DECLARE_DERIVED_STRUCTURE( STRUCTURE, BASE )

// defines a concrete object
#define REFLECT_DEFINE_BASE_STRUCTURE( STRUCTURE ) \
    _REFLECT_DEFINE_BASE_STRUCTURE( STRUCTURE )

#define REFLECT_DEFINE_DERIVED_STRUCTURE( STRUCTURE ) \
    _REFLECT_DEFINE_DERIVED_STRUCTURE( STRUCTURE  )

#include "Reflect/Structure.inl"
