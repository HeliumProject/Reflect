#pragma once

#include "Foundation/Endian.h"
#include "Foundation/Stream.h"
#include "Foundation/String.h"
#include "Foundation/Set.h"

#include "Reflect/API.h"
#include "Reflect/Object.h"
#include "Reflect/ReflectionInfo.h"

namespace Helium
{
	namespace Reflect
	{
		namespace SimpleDataTypes
		{
			enum Type
			{
				Invalid = 0,
				Boolean,
				Character,
				Integer,
				FloatingPoint,
				String,
				Crc32,
				
				Count,
				Mask = 0xF
			};
		}
		typedef SimpleDataTypes::Type SimpleDataType;

		namespace ContainerDataTypes
		{
			enum Type
			{
				None = 0,
				Set,
				Sequence,
				Associative,

				Count,
				Mask = 0xF0
			};
		}
		typedef ContainerDataTypes::Type ContainerDataType;

		class HELIUM_REFLECT_API DataHeader
		{
		public:
			inline DataHeader();

			inline void Write( Stream& stream ) const;
			inline void Read( Stream& stream );

			template< class T >
			uint32_t GetLength( uint32_t extra = 0 ) const;
			template< class T >
			uint32_t SetLength( uint32_t extra = 0 );
			
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

			template< class T >
			inline T* GetAddress( uintptr_t offsetInField = 0 ) const;

			// if we point to an object, notify the host object that it was changed (if doIt is true)
			inline void RaiseChanged( bool doIt = true ) const;

			void*			m_Instance;
			const Field*	m_Field;
		};

		//
		// A Data is an Object that knows how to read/write data
		//  from any kind of support Archive type (XML and Binary), given
		//  an address in memory to serialize/deserialize data to/from
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
		class ScalarData : Data
		{
		public:
			// binary serialization
			virtual void Serialize( DataInstance i, Stream& stream, bool raiseChanged ) = 0;
			virtual void Deserialize( DataInstance i, const Stream& stream ) = 0;

			// string serialization
			virtual void Serialize( DataInstance i, String& string, bool raiseChanged ) = 0;
			virtual void Deserialize( DataInstance i, const String& string ) = 0;
		};

		//
		// Polymorphic access to container data
		//

		class ContainerData : public Data
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::ContainerData, ContainerData, Data )

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