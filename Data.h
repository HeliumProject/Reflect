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

		class HELIUM_REFLECT_API DataInstance
		{
		public:
			// compute address from base and field combination
			inline DataInstance( const Field* field, Object* object );
			inline DataInstance( Structure* baseAddress, const Field* field, Object* object );
			inline DataInstance( void* finalAddress, const Field* field, Object* object );

			// copy constructor to support pass-by-value
			inline DataInstance( const DataInstance& rhs );

			// resolve the actual memory address of the data
			template< class T > T& As() const;

			// if we point to an object, notify the host object that it was changed (if doIt is true)
			inline void RaiseChanged( bool doIt = true ) const;

			Object*			m_Object;
			const Field*	m_Field;
			void*			m_Address;
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

			// copies value from one instance to another
			virtual bool Copy( DataInstance src, DataInstance dest, uint32_t flags = 0) = 0;

			// tests for equivalence across instances
			virtual bool Equals( DataInstance a, DataInstance b ) = 0;

			// explore contents
			virtual void Accept( DataInstance i, Visitor& visitor ) = 0;
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

			// setup type
			inline ScalarData( ScalarType t );

			// value -> string
			virtual void Print( DataInstance i, String& string, ObjectIdentifier& identifier ) = 0;

			// string -> value
			virtual void Parse( const String& string, DataInstance i, ObjectResolver& resolver, bool raiseChanged ) = 0;

			// innate type
			const ScalarType m_Type;
		};

		//
		// Polymorphic access to container data
		//

		class HELIUM_REFLECT_API ContainerData : public Data
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ContainerData, ContainerData, Data );

			virtual size_t GetSize() const = 0;
			virtual void Clear() = 0;
		};

		//
		// Set containers
		//

		class HELIUM_REFLECT_API SetData : public ContainerData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::SetData, SetData, ContainerData );

			virtual Data* GetItemData() const = 0;

			virtual void GetItems( DynamicArray< DataInstance >& items ) const = 0;

			virtual void AddItem( DataInstance value ) = 0;
			virtual void RemoveItem( DataInstance value ) = 0;
			virtual bool ContainsItem( DataInstance value ) const = 0;
		};

		//
		// Sequential containers (Dynamic Arrays, Deque, etc...)
		//

		class HELIUM_REFLECT_API SequenceData : public ContainerData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::SequenceData, SequenceData, ContainerData );

			virtual Data* GetItemData() const = 0;

			virtual void SetSize( size_t size ) = 0;

			virtual void GetItems( DynamicArray< DataInstance >& items ) const = 0;
			virtual DataInstance GetItem( size_t at ) = 0;

			virtual void SetItem( size_t at, DataInstance value ) = 0;
			virtual void Insert( size_t at, DataInstance value ) = 0;
			virtual void Remove( size_t at ) = 0;
			
			virtual void MoveUp( Set< size_t >& items ) = 0;
			virtual void MoveDown( Set< size_t >& items ) = 0;
		};

		//
		// Association containers (Map, Hashtable, Hashmap, etc...)
		//

		class HELIUM_REFLECT_API AssociationData : public ContainerData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::AssociationData, AssociationData, ContainerData );

			virtual Data* GetKeyData() const = 0;
			virtual Data* GetValueData() const = 0;

			virtual void GetItems( DynamicArray<DataInstance>& keys, DynamicArray<DataInstance>& values ) = 0;
			virtual DataInstance GetItem( DataInstance key ) = 0;

			virtual void SetItem( DataInstance key, DataInstance value ) = 0;
			virtual void RemoveItem( DataInstance key ) = 0;
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