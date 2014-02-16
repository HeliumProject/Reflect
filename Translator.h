#pragma once

#include "Foundation/Endian.h"
#include "Foundation/Stream.h"
#include "Foundation/String.h"
#include "Foundation/Name.h"
#include "Foundation/Set.h"

#include "Reflect/API.h"
#include "Reflect/Meta.h"

namespace Helium
{
	namespace Reflect
	{
		class Field;
		class ObjectIdentifier;
		class ObjectResolver;

		//
		// A pointer to some typed data (owned by the object itself or someone else
		//

		class HELIUM_REFLECT_API Pointer
		{
		public:
			Pointer();
			Pointer( const Field* field, Object* object, uint32_t index = 0 );
			Pointer( const Field* field, void* composite, Object* object, uint32_t index = 0 );
			Pointer( void *rawPtr, const Field* field = NULL, Object* object = NULL );
			Pointer( const Pointer& rhs );

			// resolve the actual memory address of the data as some specific type of data (danger!)
			template< class T > T& As();

			// if we point to an object, notify the host object that it was changed (if doIt is true)
			void RaiseChanged( bool doIt = true );

			void*        m_Address;
			const Field* m_Field;
			Object*      m_Object;
		};

		//
		// Variables are a Pointer that allocate its own instance of the Translated data type
		//

		class HELIUM_REFLECT_API Variable : public Pointer, NonCopyable
		{
		public:
			inline Variable( Translator* translator );
			inline ~Variable();

			Translator* m_Translator;
		};

		//
		// Data are a Pointer and Translator Pair
		//

		class HELIUM_REFLECT_API Data
		{
		public:
			inline Data( Pointer pointer, Translator* translator );
			inline Data( const Data& data );

			inline bool operator==( const Data& rhs );
			inline bool operator!=( const Data& rhs );

			Pointer     m_Pointer;
			Translator* m_Translator;
		};

		//
		// Translator abstraction object, worker for an entire class of data
		//

		namespace CopyFlags
		{
			enum MetaType
			{
				Notify    = 1 << 0, // emit changed events when copying data
				Shallow   = 1 << 1, // copy references only
			};
		}

		class HELIUM_REFLECT_API Translator : public Meta
		{
		public:
			HELIUM_META_DERIVED( MetaIds::Translator, Translator, Meta );

			inline Translator( size_t size );

			// get the default flags for this type of data
			virtual uint32_t GetDefaultFlags();

			// some templates to do default work
			template< class T > HELIUM_FORCEINLINE void DefaultConstruct( Pointer pointer );
			template< class T > HELIUM_FORCEINLINE void DefaultDestruct( Pointer pointer );
			template< class T > HELIUM_FORCEINLINE void DefaultCopy( Pointer src, Pointer dest, uint32_t flags );
			template< class T > HELIUM_FORCEINLINE bool DefaultEquals( Pointer a, Pointer b );

			// call the constructor (in-place)
			virtual void Construct( Pointer pointer ) = 0;

			// call the destructor (in-place)
			virtual void Destruct( Pointer pointer ) = 0;

			// copies value from one instance to another
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) = 0;

			// tests for equivalence across instances
			virtual bool Equals( Pointer a, Pointer b ) = 0;

			// sizeof(), in bytes
			const size_t m_Size;
		};

		//
		// Scalar data interface, for actual read/write support
		//

		namespace ScalarTypes
		{
			enum MetaType
			{
				Boolean,
				Unsigned8,
				Unsigned16,
				Unsigned32,
				Unsigned64,
				Signed8,
				Signed16,
				Signed32,
				Signed64,
				Float32,
				Float64,
				String,
			};
		}
		typedef ScalarTypes::MetaType ScalarType;

		class HELIUM_REFLECT_API ScalarTranslator : public Translator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::ScalarTranslator, ScalarTranslator, Translator );

			inline ScalarTranslator( size_t size, ScalarType type );

			template< class T > void DefaultPrint( Pointer pointer, String& string, ObjectIdentifier* identifier );
			template< class T > void DefaultParse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged );

			// value -> string
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier = NULL ) = 0;

			// string -> value
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver = NULL, bool raiseChanged = false ) = 0;

			const ScalarType m_Type;
		};

		//
		// Structures
		//

		class HELIUM_REFLECT_API StructureTranslator : public Translator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::StructureTranslator, StructureTranslator, Translator );

			inline StructureTranslator( size_t size );

			virtual const MetaStruct* GetMetaStruct() const = 0;
		};

		//
		// Polymorphic access to container data
		//

		class HELIUM_REFLECT_API ContainerTranslator : public Translator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::ContainerTranslator, ContainerTranslator, Translator );

			inline ContainerTranslator( size_t size );

			virtual size_t GetLength( Pointer container ) const = 0;
			virtual void   Clear( Pointer container ) = 0;
		};

		//
		// Set containers
		//

		class HELIUM_REFLECT_API SetTranslator : public ContainerTranslator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::SetTranslator, SetTranslator, ContainerTranslator );
			
			inline SetTranslator( size_t size );

			virtual Translator* GetItemTranslator() const = 0;

			virtual void        GetItems( Pointer set, DynamicArray< Pointer >& items ) const = 0;

			virtual void        InsertItem( Pointer set, Pointer item ) = 0;
			virtual void        RemoveItem( Pointer set, Pointer item ) = 0;
			virtual bool        ContainsItem( Pointer set, Pointer item ) const = 0;
		};

		//
		// Sequential containers (Dynamic Arrays, Deque, etc...)
		//

		class HELIUM_REFLECT_API SequenceTranslator : public ContainerTranslator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::SequenceTranslator, SequenceTranslator, ContainerTranslator );
			
			inline SequenceTranslator( size_t size );

			virtual Translator* GetItemTranslator() const = 0;

			virtual void        GetItems( Pointer sequence, DynamicArray< Pointer >& items ) const = 0;

			virtual void        SetLength( Pointer sequence, size_t length ) = 0;
			virtual Pointer     GetItem( Pointer sequence, size_t at ) = 0;
			virtual void        SetItem( Pointer sequence, size_t at, Pointer value ) = 0;
			virtual void        Insert( Pointer sequence, size_t at, Pointer value ) = 0;
			virtual void        Remove( Pointer sequence, size_t at ) = 0;
			virtual void        MoveUp( Pointer sequence, Set< size_t >& items ) = 0;
			virtual void        MoveDown( Pointer sequence, Set< size_t >& items ) = 0;
		};

		//
		// Association containers (Map, Hashtable, Hashmap, etc...)
		//

		class HELIUM_REFLECT_API AssociationTranslator : public ContainerTranslator
		{
		public:
			HELIUM_META_DERIVED( MetaIds::AssociationTranslator, AssociationTranslator, ContainerTranslator );
			
			inline AssociationTranslator( size_t size );

			virtual ScalarTranslator* GetKeyTranslator() const = 0;
			virtual Translator*       GetValueTranslator() const = 0;

			virtual void              GetItems( Pointer association, DynamicArray<Pointer>& keys, DynamicArray<Pointer>& values ) = 0;

			virtual Pointer           GetItem( Pointer association, Pointer key ) = 0;
			virtual void              SetItem( Pointer association, Pointer key, Pointer value ) = 0;
			virtual void              RemoveItem( Pointer association, Pointer key ) = 0;
		};

		//
		// MetaType and Translator deduction
		//  Function overloading semantics provide for easier selection of a type T than using only templates
		//   The first parameter gets overloaded with different arument types for the compiler to select the appropriate function
		//   The second parameter ensures the bare type in the template argument is available for further deduction
		//

		template< class T >
		const MetaType* DeduceKeyType();

		template< class T >
		const MetaType* DeduceValueType();

		template< class T >
		Translator* AllocateTranslator();
	}
}

#include "Reflect/Translator.inl"