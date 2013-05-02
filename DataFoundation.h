#pragma once

#include "Foundation/String.h"

#include "Reflect/Registry.h"
#include "Reflect/DataSimple.h"

namespace Helium
{
	namespace Reflect
	{
		class HELIUM_REFLECT_API StringData : public ScalarData
		{
		public:
			StringData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier ) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Data* AllocateData( const CharString&, const CharString& )
		{
			return new StringData;
		}
		inline Data* AllocateData( const WideString&, const WideString& )
		{
			throw Reflect::Exception("Wide strings are not supported");
		}

#ifdef REFLECT_REFACTOR
		class HELIUM_REFLECT_API PathData : public Data
		{
		public:
			typedef Helium::FilePath DataType;
			DataPointer< DataType > m_Data;

			virtual void ConnectData( void* data ) HELIUM_OVERRIDE;

			virtual bool Set( Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool Equals( Object* object ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>(tostream& stream) const HELIUM_OVERRIDE;
			virtual tistream& operator<<(tistream& stream) HELIUM_OVERRIDE;
		};

		template< class T >
		class HELIUM_REFLECT_API SimpleDynamicArrayData : public SequenceData
		{
		public:
			typedef DynamicArray< T > DataType;
			DataPointer< DataType > m_Data;

			virtual void ConnectData( void* data ) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void SetSize( size_t size ) HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetItemClass() const HELIUM_OVERRIDE;
			virtual DataPtr GetItem( size_t at ) HELIUM_OVERRIDE;
			virtual void SetItem( size_t at, Data* value ) HELIUM_OVERRIDE;
			virtual void Insert( size_t at, Data* value ) HELIUM_OVERRIDE;
			virtual void Remove( size_t at ) HELIUM_OVERRIDE;
			virtual void MoveUp( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;
			virtual void MoveDown( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;

			virtual bool Set( Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool Equals( Object* object ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>( tostream& stream ) const HELIUM_OVERRIDE;
			virtual tistream& operator<<( tistream& stream ) HELIUM_OVERRIDE;
		};

		template< typename KeyT, typename EqualKeyT = Equals< KeyT >, typename AllocatorT = DefaultAllocator >
		class HELIUM_REFLECT_API SimpleSetData : public SetData
		{
		public:
			typedef Helium::Set< KeyT, EqualKeyT, AllocatorT > DataType;
			DataPointer< DataType > m_Data;

			virtual void ConnectData( void* data ) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetItemClass() const HELIUM_OVERRIDE;
			virtual void GetItems( DynamicArray< DataPtr >& items ) const HELIUM_OVERRIDE;
			virtual void AddItem( Data* value ) HELIUM_OVERRIDE;
			virtual void RemoveItem( Data* value ) HELIUM_OVERRIDE;
			virtual bool ContainsItem( Data* value ) const HELIUM_OVERRIDE;

			virtual bool Set( Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool Equals( Object* object ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>( tostream& stream ) const HELIUM_OVERRIDE;
			virtual tistream& operator<<( tistream& stream ) HELIUM_OVERRIDE;

		private:
			template< class ArchiveT >
			void Serialize( ArchiveT& archive );
			template< class ArchiveT >
			void Deserialize( ArchiveT& archive );
		};

		template< typename KeyT, typename CompareKeyT = Less< KeyT >, typename AllocatorT = DefaultAllocator >
		class HELIUM_REFLECT_API SimpleSortedSetData : public SetData
		{
		public:
			typedef SortedSet< KeyT, CompareKeyT, AllocatorT > DataType;
			DataPointer< DataType > m_Data;

			virtual void ConnectData( void* data ) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetItemClass() const HELIUM_OVERRIDE;
			virtual void GetItems( DynamicArray< DataPtr >& items ) const HELIUM_OVERRIDE;
			virtual void AddItem( Data* value ) HELIUM_OVERRIDE;
			virtual void RemoveItem( Data* value ) HELIUM_OVERRIDE;
			virtual bool ContainsItem( Data* value ) const HELIUM_OVERRIDE;

			virtual bool Set( Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool Equals( Object* object ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>( tostream& stream ) const HELIUM_OVERRIDE;
			virtual tistream& operator<<( tistream& stream ) HELIUM_OVERRIDE;

		private:
			template< class ArchiveT >
			void Serialize( ArchiveT& archive );
			template< class ArchiveT >
			void Deserialize( ArchiveT& archive );
		};

		template< typename KeyT, typename ValueT, typename EqualKeyT = Equals< KeyT >, typename AllocatorT = DefaultAllocator >
		class HELIUM_REFLECT_API SimpleMapData : public AssociativeData
		{
		public:
			typedef Map< KeyT, ValueT, EqualKeyT, AllocatorT > DataType;
			DataPointer< DataType > m_Data;

			typedef SimpleMapData< KeyT, ValueT, EqualKeyT, AllocatorT > MapDataT;
			REFLECT_DECLARE_OBJECT( MapDataT, AssociativeData );

			SimpleMapData();
			~SimpleMapData();

			virtual void ConnectData( void* data ) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetKeyClass() const HELIUM_OVERRIDE;
			virtual const Class* GetValueClass() const HELIUM_OVERRIDE;
			virtual void GetItems( A_ValueType& items ) HELIUM_OVERRIDE;
			virtual DataPtr GetItem( Data* key ) HELIUM_OVERRIDE;
			virtual void SetItem( Data* key, Data* value ) HELIUM_OVERRIDE;
			virtual void RemoveItem( Data* key ) HELIUM_OVERRIDE;

			virtual bool Set( Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool Equals( Object* object ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>( tostream& stream ) const HELIUM_OVERRIDE;
			virtual tistream& operator<<( tistream& stream ) HELIUM_OVERRIDE;

		private:
			template< class ArchiveT >
			void Serialize( ArchiveT& archive );
			template< class ArchiveT >
			void Deserialize( ArchiveT& archive );
		};

		template< typename KeyT, typename ValueT, typename CompareKeyT = Less< KeyT >, typename AllocatorT = DefaultAllocator >
		class HELIUM_REFLECT_API SimpleSortedMapData : public AssociativeData
		{
		public:
			typedef SortedMap< KeyT, ValueT, CompareKeyT, AllocatorT > DataType;
			DataPointer< DataType > m_Data;

			virtual void ConnectData( void* data ) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetKeyClass() const HELIUM_OVERRIDE;
			virtual const Class* GetValueClass() const HELIUM_OVERRIDE;
			virtual void GetItems( A_ValueType& items ) HELIUM_OVERRIDE;
			virtual DataPtr GetItem( Data* key ) HELIUM_OVERRIDE;
			virtual void SetItem( Data* key, Data* value ) HELIUM_OVERRIDE;
			virtual void RemoveItem( Data* key ) HELIUM_OVERRIDE;

			virtual bool Set( Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool Equals( Object* object ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>( tostream& stream ) const HELIUM_OVERRIDE;
			virtual tistream& operator<<( tistream& stream ) HELIUM_OVERRIDE;

		private:
			template< class ArchiveT >
			void Serialize( ArchiveT& archive );
			template< class ArchiveT >
			void Deserialize( ArchiveT& archive );
		};
#endif
	}
}

#include "Reflect/DataFoundation.inl"