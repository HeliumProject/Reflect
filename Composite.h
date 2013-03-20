#pragma once

#include <typeinfo>

#include "Foundation/Attribute.h"
#include "Foundation/DynamicArray.h"
#include "Foundation/Set.h"

#include "Reflect/Type.h"
#include "Reflect/Visitor.h"

//
//  Composite Binary format:
//
//  struct Field
//  {
//      int32_t name;               // string pool index of the name of this field
//      int32_t serializer_id;      // string pool index of the name of the data type
//  };
//
//  struct Composite
//  {
//      int32_t short_name;         // string pool index of the name for this type
//      int32_t count;              // number of field infos to follow
//      Field[] fields;             // field rtti data
//      int32_t term;               // -1
//  };
//

namespace Helium
{
	namespace Reflect
	{
		class Composite;
		typedef void (*PopulateCompositeFunc)( Composite& );

		namespace FieldFlags
		{
			enum Enum
			{
				Discard     = 1 << 0,       // disposable fields are not serialized
				Force       = 1 << 1,       // forced fields are always serialized
				Share       = 1 << 2,       // shared fields are not cloned or compared deeply
				Hide        = 1 << 3,       // hidden fields are not inherently visible in UI
				ReadOnly    = 1 << 4,       // read-only fields cannot be edited in the UI inherently
			};
		}

		//
		// Field (an object of a composite)
		//

		class HELIUM_REFLECT_API Field : public PropertyCollection
		{
		public:
			Field();

			// allocate and connect to the default
			bool IsDefaultValue( void* instance ) const;

			// determine if this field should be serialized
			bool ShouldSerialize( void* instance ) const;

			const Composite*        m_Composite;    // the type we are a field of
			const tchar_t*          m_Name;         // name of this field
			uint32_t                m_Size;         // the size of this field
			uintptr_t               m_Offset;       // the offset to the field
			uint32_t                m_Flags;        // flags for special behavior
			uint32_t                m_Index;        // the unique id of this field
			const Type*             m_Type;         // the type of this field (NULL for POD types)
			Data*					m_Data;			// type id of the data to use
		};

		//
		// Empty struct just for type deduction purposes (for stand alone structs, not Object classes)
		//  don't worry though, even though this class is non-zero in size on its own,
		//  your derived struct type can use the memory this takes due to C/C++ standard
		//  'Empty Base Optimization'
		//

		struct HELIUM_REFLECT_API Structure
		{
		};

		//
		// Composite (C++ `struct` or `class`)
		//

		class HELIUM_REFLECT_API Composite : public Type
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::Composite, Composite, Type );

		protected:
			Composite();
			~Composite();

		public:
			// protect external allocation to keep inlined code in this dll
			static Composite* Create();

			// creator for structure types
			template< class StructureT >
			static void Create( Composite const*& pointer, const tchar_t* name, const tchar_t* baseName );

			// shared logic with class types
			template< class CompositeT >
			static void Create( const tchar_t* name, const tchar_t* baseName, PopulateCompositeFunc populate, Composite* info );

			// overloaded functions from Type
			virtual void Register() const HELIUM_OVERRIDE;
			virtual void Unregister() const HELIUM_OVERRIDE;

			// inheritance hierarchy
			bool IsType(const Composite* type) const;
			void AddDerived( const Composite* derived ) const;
			void RemoveDerived( const Composite* derived ) const;

			// Compare two composite instances of *this* type
			bool Equals( void* a, void* b ) const;

			// visits fields recursively, used to interactively traverse structures
			void Visit( void* instance, Visitor& visitor ) const;

			// copies data from one instance to another by finding a common base class and cloning all of the fields from the source object into the destination object.
			void Copy( void* source, void* destination ) const;

			// find a field in this composite
			const Field* FindFieldByName(uint32_t crc) const;
			const Field* FindFieldByIndex(uint32_t index) const;
			const Field* FindFieldByOffset(uint32_t offset) const;

			// finds the field info given a pointer to a member variable on a class.
			template< class CompositeT, typename FieldT >
			const Field* FindField( FieldT CompositeT::* pointerToMember ) const;

			// computes the number of fields in all our base classes (the base index for our fields)
			uint32_t GetBaseFieldCount() const;

			// concrete field population functions, called from template functions below with deducted data
			Reflect::Field* AddField( const tchar_t* name, const uint32_t offset, uint32_t size, Data* data, const Type* type = NULL, int32_t flags = 0 );

			// compute the offset from the 'this' pointer for the specified pointer-to-member
			template < class CompositeT, class FieldT >
			static inline uint32_t GetOffset( FieldT CompositeT::* field );

			// deduce and allocate the appropriate data object and append field data to the composite
			template < class CompositeT, class FieldT >
			inline Reflect::Field* AddField( FieldT CompositeT::* field, const tchar_t* name, int32_t flags = 0, Data* data = AllocateData<FieldT>() );

		public:
			const Composite*                        m_Base;                 // the base type name
			mutable const Composite*                m_FirstDerived;         // head of the derived linked list, mutable since its populated by other objects
			mutable const Composite*                m_NextSibling;          // next in the derived linked list, mutable since its populated by other objects
			DynamicArray< Field >                   m_Fields;               // fields in this composite
			PopulateCompositeFunc                   m_Populate;             // function to populate this structure
			void*                                   m_Default;              // default instance
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
static const Helium::Reflect::Composite* CreateComposite(); \
static const Helium::Reflect::Composite* s_Composite; \
static Helium::Reflect::StructureRegistrar< STRUCTURE, void > s_Registrar;

#define _REFLECT_DECLARE_DERIVED_STRUCTURE( STRUCTURE, BASE ) \
public: \
typedef BASE Base; \
typedef STRUCTURE This; \
static const Helium::Reflect::Composite* CreateComposite(); \
static const Helium::Reflect::Composite* s_Composite; \
static Helium::Reflect::StructureRegistrar< STRUCTURE, BASE > s_Registrar;

// defines the static type info vars
#define _REFLECT_DEFINE_BASE_STRUCTURE( STRUCTURE ) \
const Helium::Reflect::Structure* STRUCTURE::CreateComposite() \
{ \
	HELIUM_ASSERT( s_Composite == NULL ); \
	Helium::Reflect::Structure::Create<STRUCTURE>( s_Composite, TXT( #STRUCTURE ), NULL ); \
	return s_Composite; \
} \
const Helium::Reflect::Structure* STRUCTURE::s_Composite = NULL; \
Helium::Reflect::StructureRegistrar< STRUCTURE, void > STRUCTURE::s_Registrar( TXT( #STRUCTURE ) );

#define _REFLECT_DEFINE_DERIVED_STRUCTURE( STRUCTURE ) \
const Helium::Reflect::Structure* STRUCTURE::CreateComposite() \
{ \
	HELIUM_ASSERT( s_Composite == NULL ); \
	HELIUM_ASSERT( STRUCTURE::Base::s_Composite != NULL ); \
	Helium::Reflect::Structure::Create<STRUCTURE>( s_Composite, TXT( #STRUCTURE ), STRUCTURE::Base::s_Composite->m_Name ); \
	return s_Composite; \
} \
const Helium::Reflect::Composite* STRUCTURE::s_Composite = NULL; \
Helium::Reflect::StructureRegistrar< STRUCTURE, STRUCTURE::Base > STRUCTURE::s_Registrar( TXT( #STRUCTURE ) );

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

#include "Reflect/Composite.inl"
