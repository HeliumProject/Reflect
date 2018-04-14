#pragma once

#include "Platform/Types.h"
#include "Platform/Utility.h"

#include "Foundation/Attribute.h"
#include "Foundation/DynamicArray.h"
#include "Foundation/Set.h"

#include "Reflect/MetaType.h"
#include "Reflect/Registry.h"
#include "Reflect/Translator.h"

namespace Helium
{
	namespace Reflect
	{
		class MetaStruct;
		typedef void (*PopulateMetaTypeFunc)( MetaStruct& );
		typedef void (*DefaultDeleteFunc)( void* );

		namespace FieldFlags
		{
			enum MetaType
			{
				Discard     = 1 << 0,       // disposable fields are not serialized
				Force       = 1 << 1,       // forced fields are always serialized
				Share       = 1 << 2,       // shared fields are not cloned or compared deeply
				Hide        = 1 << 3,       // hidden fields are not inherently visible in UI
				ReadOnly    = 1 << 4,       // read-only fields cannot be edited in the UI inherently
			};
		}

		//
		// Field (member data of a composite)
		//

		class HELIUM_REFLECT_API Field : public PropertyCollection
		{
		public:
			Field();

			// allocate and connect to the default
			bool IsDefaultValue( void* address, Object* object, uint32_t index = 0 ) const;

			// determine if this field should be serialized
			bool ShouldSerialize( void* address, Object* object, uint32_t index = 0 ) const;

			const MetaStruct*      m_Structure;  // the type we are a field of
			const char*            m_Name;       // name of this field
			uint32_t               m_NameCrc;    // precompute the CRC of the name for efficiency
			uint32_t               m_Size;       // the size of this field
			uint32_t               m_Count;      // the static array size
			uint32_t               m_Offset;     // the offset to the field
			uint32_t               m_Flags;      // flags for special behavior
			uint32_t               m_Index;      // the unique id of this field
			const MetaType*        m_KeyType;    // the key type, if any, of the internal data
			const MetaType*        m_ValueType;  // the value type, if any, of the internal data
			SmartPtr< Translator > m_Translator; // interface to the data
		};

		//
		// Method (member function of a composite)
		//

		class HELIUM_REFLECT_API Method : public PropertyCollection
		{
		public:
			typedef SmartPtr< Delegate< void* >::DelegateImpl > DelegateImplPtr;

			Method();

			const MetaStruct*      m_Structure;    // the type we are a field of
			const char*            m_Name;         // name of this field
			SmartPtr< Translator > m_Translator;   // the argument type
			DelegateImplPtr        m_Delegate;     // the delegate to invoke the call
		};

		//
		// Empty struct just for type deduction purposes (for stand alone structs, not Object classes)
		//  don't worry though, even though this class is non-zero in size on its own,
		//  your derived struct type can use the memory this takes due to C/C++ standard
		//  'Empty Base Optimization'
		//

		struct HELIUM_REFLECT_API Struct
		{
		};

		//
		// MetaStruct (C++ `struct`)
		//

		class HELIUM_REFLECT_API MetaStruct : public MetaType
		{
		public:
			HELIUM_META_DERIVED( MetaIds::MetaStruct, MetaStruct, MetaType );

		protected:
			MetaStruct();
			~MetaStruct();

		public:
			// protect external allocation to keep inlined code in this dll
			static MetaStruct* Create();

			// creator for structure types
			template< class StructureT >
			static void Create( MetaStruct const*& pointer, const char* name, const char* baseName );

			// shared logic with class types
			template< class StructureT >
			static void Create( const char* name, const char* baseName, PopulateMetaTypeFunc populate, MetaStruct* info );

			// overloaded functions from MetaType
			virtual void Register() const override;
			virtual void Unregister() const override;

			// inheritance hierarchy
			bool IsType(const MetaStruct* type) const;
			void AddDerived( const MetaStruct* derived ) const;
			void RemoveDerived( const MetaStruct* derived ) const;

			// Compare two composite instances of *this* type
			bool Equals( void* compositeA, Object* objectA, void* compositeB, Object* objectB ) const;

			// copies data from one instance to another by finding a common base class and cloning all of the fields from the source object into the destination object.
			void Copy( void* compositeSource, Object* objectSource, void* compositeDestination, Object* objectDestination, bool shallowCopy = false ) const;

			// find a field in this composite
			const Field* FindFieldByName(uint32_t crc) const;
			const Field* FindFieldByIndex(uint32_t index) const;
			const Field* FindFieldByOffset(uint32_t offset) const;

			// finds the field info given a pointer to a member variable on a class.
			template< class StructureT, typename FieldT >
			const Field* FindField( FieldT StructureT::* pointerToMember ) const;

			// computes the number of fields in all our base classes (the base index for our fields)
			uint32_t GetBaseFieldCount() const;

			// concrete field population functions, called from template functions below with deducted data
			Reflect::Field* AllocateField();
			Reflect::Method* AllocateMethod();

			// compute the offset from the 'this' pointer for the specified pointer-to-member
			template < class StructureT, class FieldT >
			static inline uint32_t GetOffset( FieldT StructureT::* field );

			// compute the count of a static array
			template < class T, size_t N >
			static inline uint32_t GetArrayCount( T (&array)[N] );

			// compute the count of a normal type (1)
			template < class T >
			static inline uint32_t GetCount( std::false_type /*is_array*/ );

			// compute the count of an array type (1)
			template < class T >
			static inline uint32_t GetCount( std::true_type  /*is_array*/  );

			// deduce key type
			template < class T >
			static inline const MetaType* DeduceKeyType( std::false_type /*is_array*/ );
			template < class T >
			static inline const MetaType* DeduceKeyType( std::true_type  /*is_array*/  );

			// deduce value type
			template < class T >
			static inline const MetaType* DeduceValueType( std::false_type /*is_array*/ );
			template < class T >
			static inline const MetaType* DeduceValueType( std::true_type  /*is_array*/  );

			// create translator object
			template < class T >
			static inline Translator* AllocateTranslator( std::false_type /*is_array*/ );
			template < class T >
			static inline Translator* AllocateTranslator( std::true_type  /*is_array*/  );

			// deduce and allocate the appropriate translator object and append field data to the composite
			template < class StructureT, class FieldT >
			Reflect::Field* AddField( FieldT StructureT::* field, const char* name, uint32_t flags = 0, Translator* translator = NULL );

			// append method data to the composite
			template < class StructureT, class ArgumentT >
			Reflect::Method* AddMethod( void (StructureT::*method)( ArgumentT& ), const char* name );

		public:
			const MetaStruct*         m_Base;         // the base type name
			mutable const MetaStruct* m_FirstDerived; // head of the derived linked list, mutable since its populated by other objects
			mutable const MetaStruct* m_NextSibling;  // next in the derived linked list, mutable since its populated by other objects
			DynamicArray< Field >     m_Fields;       // fields in this composite
			DynamicArray< Method >    m_Methods;      // methods in this composite
			PopulateMetaTypeFunc      m_Populate;     // function to populate this structure
			void*                     m_Default;      // default instance
			DefaultDeleteFunc         m_DefaultDelete;// function to use to delete the default instance
		};

		template< class ClassT, class BaseT >
		class MetaStructRegistrar : public MetaTypeRegistrar
		{
		public:
			MetaStructRegistrar(const char* name);
			~MetaStructRegistrar();

			virtual void Register();
			virtual void Unregister();
		};

		template< class ClassT >
		class MetaStructRegistrar< ClassT, void > : public MetaTypeRegistrar
		{
		public:
			MetaStructRegistrar(const char* name);
			~MetaStructRegistrar();

			virtual void Register();
			virtual void Unregister();
		};
	}
}

// declares type checking functions
#define _REFLECT_DECLARE_BASE_STRUCT( STRUCTURE ) \
public: \
typedef STRUCTURE This; \
static const Helium::Reflect::MetaStruct* CreateMetaStruct(); \
static const Helium::Reflect::MetaStruct* s_MetaStruct; \
static Helium::Reflect::MetaStructRegistrar< STRUCTURE, void > s_Registrar;

#define _REFLECT_DECLARE_DERIVED_STRUCT( STRUCTURE, BASE ) \
public: \
typedef BASE Base; \
typedef STRUCTURE This; \
static const Helium::Reflect::MetaStruct* CreateMetaStruct(); \
static const Helium::Reflect::MetaStruct* s_MetaStruct; \
static Helium::Reflect::MetaStructRegistrar< STRUCTURE, BASE > s_Registrar;

// defines the static type info vars
#define _REFLECT_DEFINE_BASE_STRUCT( STRUCTURE ) \
const Helium::Reflect::MetaStruct* STRUCTURE::CreateMetaStruct() \
{ \
	HELIUM_ASSERT( s_MetaStruct == NULL ); \
	Helium::Reflect::MetaStruct::Create<STRUCTURE>( s_MetaStruct, #STRUCTURE, NULL ); \
	return s_MetaStruct; \
} \
const Helium::Reflect::MetaStruct* STRUCTURE::s_MetaStruct = NULL; \
Helium::Reflect::MetaStructRegistrar< STRUCTURE, void > STRUCTURE::s_Registrar( #STRUCTURE );

#define _REFLECT_DEFINE_DERIVED_STRUCT( STRUCTURE ) \
const Helium::Reflect::MetaStruct* STRUCTURE::CreateMetaStruct() \
{ \
	HELIUM_ASSERT( s_MetaStruct == NULL ); \
	HELIUM_ASSERT( STRUCTURE::Base::s_MetaStruct != NULL ); \
	Helium::Reflect::MetaStruct::Create<STRUCTURE>( s_MetaStruct, #STRUCTURE, STRUCTURE::Base::s_MetaStruct->m_Name ); \
	return s_MetaStruct; \
} \
const Helium::Reflect::MetaStruct* STRUCTURE::s_MetaStruct = NULL; \
Helium::Reflect::MetaStructRegistrar< STRUCTURE, STRUCTURE::Base > STRUCTURE::s_Registrar( #STRUCTURE );

// declares a concrete object with creator
#define HELIUM_DECLARE_BASE_STRUCT( STRUCTURE ) \
	_REFLECT_DECLARE_BASE_STRUCT( STRUCTURE )

#define HELIUM_DECLARE_DERIVED_STRUCT( STRUCTURE, BASE ) \
	_REFLECT_DECLARE_DERIVED_STRUCT( STRUCTURE, BASE )

// defines a concrete object
#define HELIUM_DEFINE_BASE_STRUCT( STRUCTURE ) \
	_REFLECT_DEFINE_BASE_STRUCT( STRUCTURE )

#define HELIUM_DEFINE_DERIVED_STRUCT( STRUCTURE ) \
	_REFLECT_DEFINE_DERIVED_STRUCT( STRUCTURE  )

#include "Reflect/MetaStruct.inl"
