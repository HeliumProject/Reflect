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

		class HELIUM_REFLECT_API DataPointer
		{
		public:
			// compute address from base and field combination
			inline DataPointer( const Field* field, Object* object, uint32_t index = 0 );
			inline DataPointer( const Field* field, void* composite, Object* object, uint32_t index = 0 );
			inline DataPointer( const DataPointer& rhs );

			// resolve the actual memory address of the data
			template< class T > T& As();

			// if we point to an object, notify the host object that it was changed (if doIt is true)
			inline void RaiseChanged( bool doIt = true );

			void*        m_Address;
			const Field* m_Field;
			Object*      m_Object;
		};

		class Data;

		class HELIUM_REFLECT_API DataVariable : public DataPointer, NonCopyable
		{
		public:
			inline DataVariable( Data* data );
			inline ~DataVariable();

			Data*      m_Data;
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
		// Data abstraction object, worker for an entire class of data
		//

		namespace DataFlags
		{
			enum Type
			{
				Shallow   = 1 << 0,
			};
		}

		class HELIUM_REFLECT_API Data : public ReflectionInfo
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::Data, Data, ReflectionInfo );

			inline Data( size_t size );

			template< class T > HELIUM_FORCEINLINE void DefaultConstruct( DataPointer pointer );
			template< class T > HELIUM_FORCEINLINE void DefaultDestruct( DataPointer pointer );
			template< class T > HELIUM_FORCEINLINE bool DefaultCopy( DataPointer src, DataPointer dest, uint32_t flags );
			template< class T > HELIUM_FORCEINLINE bool DefaultEquals( DataPointer a, DataPointer b );
			template< class T > HELIUM_FORCEINLINE void DefaultAccept( DataPointer p, Visitor& visitor );

			// call the constructor (in-place)
			virtual void Construct( DataPointer pointer ) = 0;

			// call the destructor (in-place)
			virtual void Destruct( DataPointer pointer ) = 0;

			// copies value from one instance to another
			virtual bool Copy( DataPointer src, DataPointer dest, uint32_t flags = 0 ) = 0;

			// tests for equivalence across instances
			virtual bool Equals( DataPointer a, DataPointer b ) = 0;

			// explore contents
			virtual void Accept( DataPointer p, Visitor& visitor ) = 0;

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

		class HELIUM_REFLECT_API ScalarData : public Data
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ScalarData, ScalarData, Data );

			inline ScalarData( size_t size, ScalarType type );

			template< class T > void DefaultPrint( DataPointer pointer, String& string, ObjectIdentifier& identifier );
			template< class T > void DefaultParse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged );

			// value -> string
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier ) = 0;

			// string -> value
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) = 0;

			const ScalarType m_Type;
		};

		//
		// Polymorphic access to container data
		//

		class HELIUM_REFLECT_API ContainerData : public Data
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ContainerData, ContainerData, Data );

			virtual size_t GetLength( DataPointer container ) const = 0;
			virtual void   Clear( DataPointer container ) = 0;
		};

		//
		// Set containers
		//

		class HELIUM_REFLECT_API SetData : public ContainerData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::SetData, SetData, ContainerData );

			virtual Data* GetItemData() const = 0;

			virtual void  GetItems( DataPointer set, DynamicArray< DataPointer >& items ) const = 0;

			virtual void  InsertItem( DataPointer set, DataPointer item ) = 0;
			virtual void  RemoveItem( DataPointer set, DataPointer item ) = 0;
			virtual bool  ContainsItem( DataPointer set, DataPointer item ) const = 0;
		};

		//
		// Sequential containers (Dynamic Arrays, Deque, etc...)
		//

		class HELIUM_REFLECT_API SequenceData : public ContainerData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::SequenceData, SequenceData, ContainerData );

			virtual Data*       GetItemData() const = 0;

			virtual void        GetItems( DataPointer sequence, DynamicArray< DataPointer >& items ) const = 0;

			virtual void        SetLength( DataPointer sequence, size_t length ) = 0;
			virtual DataPointer GetItem( DataPointer sequence, size_t at ) = 0;
			virtual void        SetItem( DataPointer sequence, size_t at, DataPointer value ) = 0;
			virtual void        Insert( DataPointer sequence, size_t at, DataPointer value ) = 0;
			virtual void        Remove( DataPointer sequence, size_t at ) = 0;
			virtual void        MoveUp( DataPointer sequence, Set< size_t >& items ) = 0;
			virtual void        MoveDown( DataPointer sequence, Set< size_t >& items ) = 0;
		};

		//
		// Association containers (Map, Hashtable, Hashmap, etc...)
		//

		class HELIUM_REFLECT_API AssociationData : public ContainerData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::AssociationData, AssociationData, ContainerData );

			virtual Data*       GetKeyData() const = 0;
			virtual Data*       GetValueData() const = 0;

			virtual void        GetItems( DataPointer association, DynamicArray<DataPointer>& keys, DynamicArray<DataPointer>& values ) = 0;

			virtual DataPointer GetItem( DataPointer association, DataPointer key ) = 0;
			virtual void        SetItem( DataPointer association, DataPointer key, DataPointer value ) = 0;
			virtual void        RemoveItem( DataPointer association, DataPointer key ) = 0;
		};

		//
		// Data class allocation
		//  Function overloading semantics provide for easier selection of a type T than using only templates
		//   The first parameter gets overloaded with different arument types for the compiler to select the appropriate function
		//   The second parameter ensures the bare type in the template argument is available for further deduction
		//

		template< class T >
		Data* AllocateData()
		{
			T t = T ();
			return AllocateData( t, t );
		}
	}
}

#include "Reflect/Data.inl"