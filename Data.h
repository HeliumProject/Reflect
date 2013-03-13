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
		namespace ScalarDataTypes
		{
			enum Type
			{
				Invalid = 0,
				Boolean,
				Integer,
				FloatingPoint,
				Character,
				String,
				Count,
			};
		}
		typedef ScalarDataTypes::Type ScalarDataType;

		namespace ContainerDataTypes
		{
			enum Type
			{
				Invalid = 0,
				Set,
				Sequence,
				Associative,
				Count,
			};
		}
		typedef ContainerDataTypes::Type ContainerDataType;

		class HELIUM_REFLECT_API DataHeader
		{
		public:
			inline DataHeader();
			inline bool operator==( const DataHeader& rhs ) const;
			inline void Serialize( Stream& stream ) const;
			inline void Deserialize( Stream& stream );

			template< class T > uint32_t GetLength( uint32_t extra = 0 ) const;
			template< class T > uint32_t SetLength( uint32_t extra = 0 );
			
			uint32_t	m_Length;
			uint8_t		m_ContainerType;
			uint8_t		m_ValueType;
			uint8_t		m_KeyType;
			uint8_t		m_Pad;
		};
		HELIUM_COMPILE_ASSERT( sizeof( DataHeader ) == 8 );

		//
		// A pointer to some typed data (owned by the object itself or someone else
		//

		class Field;

		class HELIUM_REFLECT_API DataInstance
		{
		public:
			inline DataInstance();
			inline DataInstance( void* instance, const Field* field );

			// resolve the actual memory address of the data
			template< class T > T* GetAddress( uintptr_t offsetInField = 0 ) const;

			// if we point to an object, notify the host object that it was changed (if doIt is true)
			inline void RaiseChanged( bool doIt = true ) const;

			void*			m_Instance;
			const Field*	m_Field;
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
			virtual void Resolve( const Name& identity, ObjectPtr& object, const Class* oointerClass ) = 0;

			// helper to extract the class of the pointer
			template< class T > void Resolve( const Name& identity, StrongPtr< T >& object );
		};

		//
		// Resolver class that defers resolution until a later time (after the objects have been loaded)
		//

		class HELIUM_REFLECT_API DeferredResolver : public ObjectResolver
		{
		public:
			virtual void Resolve( const Name& identity, ObjectPtr& object, const Class* pointerClass ) HELIUM_OVERRIDE;

		protected:
			struct Entry
			{
				Entry()
					: m_Pointer( NULL )
					, m_PointerClass( NULL )
				{

				}

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

		class Data : public ReflectionInfo
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

		class ScalarData : public Data
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ScalarData, ScalarData, Data );

			// binary serialization
			virtual void Serialize( DataInstance i, Stream& stream, ObjectIdentifier& identifier ) = 0;
			virtual void Deserialize( DataInstance i, Stream& stream, ObjectResolver& resolver, bool raiseChanged ) = 0;

			// string serialization
			virtual void Serialize( DataInstance i, String& string, ObjectIdentifier& identifier ) = 0;
			virtual void Deserialize( DataInstance i, const String& string, ObjectResolver& resolver, bool raiseChanged ) = 0;
		};

		//
		// Polymorphic access to container data
		//

		class ContainerData : public Data
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ContainerData, ContainerData, Data );

			virtual size_t GetSize() const = 0;
			virtual void Clear() = 0;
		};

		//
		// Set containers
		//

		class SetData : public ContainerData
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

		class SequenceData : public ContainerData
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
		// Associative containers (Map, Hashtable, Hashmap, etc...)
		//

		class AssociativeData : public ContainerData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::AssociativeData, AssociativeData, ContainerData );

			virtual Data* GetKeyData() const = 0;
			virtual Data* GetValueData() const = 0;

			virtual void GetItems( DynamicArray<DataInstance>& keys, DynamicArray<DataInstance>& values ) = 0;
			virtual DataInstance GetItem( DataInstance key ) = 0;

			virtual void SetItem( DataInstance key, DataInstance value ) = 0;
			virtual void RemoveItem( DataInstance key ) = 0;
		};

		//
		// Data class deduction function
		//  This function gets overloaded with different arument types to select the appropriate data class
		//  Function overloading semantics provide for easier selection of a type T than using templates
		//

		template< class T >
		Data* AllocateData()
		{
			T t;
			return AllocateData( t );
		}
	}
}

#include "Reflect/Data.inl"