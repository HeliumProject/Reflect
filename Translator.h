#pragma once

#include "Foundation/Endian.h"
#include "Foundation/Stream.h"
#include "Foundation/String.h"
#include "Foundation/Name.h"
#include "Foundation/Set.h"

#include "Reflect/API.h"
#include "Reflect/Object.h"
#include "Reflect/ReflectionInfo.h"

namespace Helium
{
	namespace Reflect
	{
		//
		// A pointer to some typed data (owned by the object itself or someone else
		//

		class Field;

		class HELIUM_REFLECT_API Pointer
		{
		public:
			inline Pointer();
			inline Pointer( const Field* field, Object* object, uint32_t index = 0 );
			inline Pointer( const Field* field, void* composite, Object* object, uint32_t index = 0 );
			inline Pointer( void *rawPtr, const Field* field = NULL, Object* object = NULL );
			inline Pointer( const Pointer& rhs );

			// resolve the actual memory address of the data as some specific type of data (danger!)
			template< class T > T& As();

			// if we point to an object, notify the host object that it was changed (if doIt is true)
			inline void RaiseChanged( bool doIt = true );

			void*        m_Address;
			const Field* m_Field;
			Object*      m_Object;
		};

		class Translator;

		class HELIUM_REFLECT_API Variable : public Pointer, NonCopyable
		{
		public:
			inline Variable( Translator* translator );
			inline ~Variable();

			SmartPtr< Translator > m_Translator;
		};

		//
		// Specifies an identifier for an object
		//

		class HELIUM_REFLECT_API ObjectIdentifier
		{
		public:
			virtual void Identify( Object* object, Name& identity ) = 0;
		};

		//
		// Resolves an identifier to an object instance
		//

		class HELIUM_REFLECT_API ObjectResolver
		{
		public:
			virtual void Resolve( const Name& identity, ObjectPtr& pointer, const Class* pointerClass ) = 0;

			// helper to extract the class of the pointer
			template< class T > void Resolve( const Name& identity, StrongPtr< T >& object );
		};

		//
		// Resolver class that defers resolution until a later time (after the objects have been loaded)
		//

		class HELIUM_REFLECT_API DeferredResolver : public ObjectResolver
		{
		public:
			virtual void Resolve( const Name& identity, ObjectPtr& pointer, const Class* pointerClass ) HELIUM_OVERRIDE;

		protected:
			struct Entry
			{
				inline Entry();

				ObjectPtr*	 m_Pointer;
				const Class* m_PointerClass;
				Name         m_Identity;
			};
			DynamicArray< Entry > m_Entries;
		};

		//
		// Translator abstraction object, worker for an entire class of data
		//

		namespace CopyFlags
		{
			enum Type
			{
				Notify    = 1 << 0, // emit changed events when copying data
				Shallow   = 1 << 1, // copy references only
			};
		}

		class HELIUM_REFLECT_API Translator : public ReflectionInfo
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::Translator, Translator, ReflectionInfo );

			inline Translator( size_t size );

			template< class T > HELIUM_FORCEINLINE void DefaultConstruct( Pointer pointer );
			template< class T > HELIUM_FORCEINLINE void DefaultDestruct( Pointer pointer );
			template< class T > HELIUM_FORCEINLINE void DefaultCopy( Pointer src, Pointer dest, uint32_t flags );
			template< class T > HELIUM_FORCEINLINE bool DefaultEquals( Pointer a, Pointer b );
			template< class T > HELIUM_FORCEINLINE void DefaultAccept( Pointer p, Visitor& visitor );

			// call the constructor (in-place)
			virtual void Construct( Pointer pointer ) = 0;

			// call the destructor (in-place)
			virtual void Destruct( Pointer pointer ) = 0;

			// copies value from one instance to another
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) = 0;

			// tests for equivalence across instances
			virtual bool Equals( Pointer a, Pointer b ) = 0;

			// explore contents
			virtual void Accept( Pointer p, Visitor& visitor ) = 0;

			// sizeof(), in bytes
			const size_t m_Size;
		};

		//
		// Scalar data interface, for actual read/write support
		//

		namespace ScalarTypes
		{
			enum Type
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
		typedef ScalarTypes::Type ScalarType;

		class HELIUM_REFLECT_API ScalarTranslator : public Translator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ScalarTranslator, ScalarTranslator, Translator );

			inline ScalarTranslator( size_t size, ScalarType type );

			template< class T > void DefaultPrint( Pointer pointer, String& string, ObjectIdentifier& identifier );
			template< class T > void DefaultParse( const String& string, Pointer pointer, ObjectResolver& resolver, bool raiseChanged );

			// value -> string
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier& identifier ) = 0;

			// string -> value
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver& resolver, bool raiseChanged ) = 0;

			const ScalarType m_Type;
		};

		//
		// Structures
		//

		class HELIUM_REFLECT_API StructureTranslator : public Translator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::StructureTranslator, StructureTranslator, Translator );

			inline StructureTranslator( size_t size );

			virtual const Structure* GetStructure() const = 0;
		};

		//
		// Polymorphic access to container data
		//

		class HELIUM_REFLECT_API ContainerTranslator : public Translator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ContainerTranslator, ContainerTranslator, Translator );

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
			REFLECTION_TYPE( ReflectionTypes::SetTranslator, SetTranslator, ContainerTranslator );
			
			inline SetTranslator( size_t size );

			virtual Translator* GetItemTranslator() const = 0;

			virtual void  GetItems( Pointer set, DynamicArray< Pointer >& items ) const = 0;

			virtual void  InsertItem( Pointer set, Pointer item ) = 0;
			virtual void  RemoveItem( Pointer set, Pointer item ) = 0;
			virtual bool  ContainsItem( Pointer set, Pointer item ) const = 0;
		};

		//
		// Sequential containers (Dynamic Arrays, Deque, etc...)
		//

		class HELIUM_REFLECT_API SequenceTranslator : public ContainerTranslator
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::SequenceTranslator, SequenceTranslator, ContainerTranslator );
			
			inline SequenceTranslator( size_t size );

			virtual Translator*       GetItemTranslator() const = 0;

			virtual void        GetItems( Pointer sequence, DynamicArray< Pointer >& items ) const = 0;

			virtual void        SetLength( Pointer sequence, size_t length ) = 0;
			virtual Pointer GetItem( Pointer sequence, size_t at ) = 0;
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
			REFLECTION_TYPE( ReflectionTypes::AssociationTranslator, AssociationTranslator, ContainerTranslator );
			
			inline AssociationTranslator( size_t size );

			virtual Translator*       GetKeyTranslator() const = 0;
			virtual Translator*       GetValueTranslator() const = 0;

			virtual void        GetItems( Pointer association, DynamicArray<Pointer>& keys, DynamicArray<Pointer>& values ) = 0;

			virtual Pointer GetItem( Pointer association, Pointer key ) = 0;
			virtual void        SetItem( Pointer association, Pointer key, Pointer value ) = 0;
			virtual void        RemoveItem( Pointer association, Pointer key ) = 0;
		};

		//
		// Translator class allocation
		//  Function overloading semantics provide for easier selection of a type T than using only templates
		//   The first parameter gets overloaded with different arument types for the compiler to select the appropriate function
		//   The second parameter ensures the bare type in the template argument is available for further deduction
		//

		template< class T >
		Translator* AllocateTranslator()
		{
			T t = T ();
			return AllocateTranslator( t, t );
		}
	}
}

#include "Reflect/Translator.inl"