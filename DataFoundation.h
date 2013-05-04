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

        // NOTE: Pretty much all of these members could be const provided necessary functions on m_InternalData are const
		template <class T>
		class SimpleDynamicArrayData : public SequenceData
		{
		public:
			SimpleDynamicArrayData();
			virtual ~SimpleDynamicArrayData();

			// Data
			virtual void        Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( DataPointer src, DataPointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void        Accept( DataPointer p, Visitor& visitor ) HELIUM_OVERRIDE;

			// ContainerData
			virtual size_t      GetLength( DataPointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( DataPointer container ) HELIUM_OVERRIDE;

			// SequenceData
			virtual Data*       GetItemData() const HELIUM_OVERRIDE;
			virtual void        GetItems( DataPointer sequence, DynamicArray< DataPointer >& items ) const HELIUM_OVERRIDE;
			virtual void        SetLength( DataPointer sequence, size_t length ) HELIUM_OVERRIDE;
			virtual DataPointer GetItem( DataPointer sequence, size_t at ) HELIUM_OVERRIDE;
			virtual void        SetItem( DataPointer sequence, size_t at, DataPointer value ) HELIUM_OVERRIDE;
			virtual void        Insert( DataPointer sequence, size_t at, DataPointer value ) HELIUM_OVERRIDE;
			virtual void        Remove( DataPointer sequence, size_t at ) HELIUM_OVERRIDE;
			virtual void        MoveUp( DataPointer sequence, Set< size_t >& items ) HELIUM_OVERRIDE;
			virtual void        MoveDown( DataPointer sequence, Set< size_t >& items ) HELIUM_OVERRIDE;

		private:
			void                SwapInternalValues(DataPointer sequence, size_t a, size_t b);

			Data*               m_InternalData;
		};
		
		template <class T>
		inline Data* AllocateData( const DynamicArray<T>&, const DynamicArray<T>& )
		{
			return new SimpleDynamicArrayData<T>();
		}

        //////////////////////////////////////////////////////////////////////////

        template <class T>
		class SimpleSetData : public SetData
		{
		public:
			SimpleSetData();
			virtual ~SimpleSetData();

			// Data
			virtual void        Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( DataPointer src, DataPointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void        Accept( DataPointer p, Visitor& visitor ) HELIUM_OVERRIDE;

			// ContainerData
			virtual size_t      GetLength( DataPointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( DataPointer container ) HELIUM_OVERRIDE;

			// SetData
			virtual Data*       GetItemData() const HELIUM_OVERRIDE;
			virtual void        GetItems( DataPointer set, DynamicArray< DataPointer >& items ) const HELIUM_OVERRIDE;
			virtual void        InsertItem( DataPointer set, DataPointer item ) HELIUM_OVERRIDE;
			virtual void        RemoveItem( DataPointer set, DataPointer item ) HELIUM_OVERRIDE;
			virtual bool        ContainsItem( DataPointer set, DataPointer item ) const HELIUM_OVERRIDE;

		private:
			Data*               m_InternalData;
		};
		
		template <class T>
		inline Data* AllocateData( const Set<T>&, const Set<T>& )
		{
			return new SimpleSetData<T>();
		}





        
        template <class KeyT, class ValueT>
        class SimpleMapData : public AssociationData
		{
		public:
			SimpleMapData();
            virtual ~SimpleMapData();

            // Data
			virtual void        Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( DataPointer src, DataPointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void        Accept( DataPointer p, Visitor& visitor ) HELIUM_OVERRIDE;

            // ContainerData
			virtual size_t      GetLength( DataPointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( DataPointer container ) HELIUM_OVERRIDE;

            // SetData
			virtual Data*       GetKeyData() const HELIUM_OVERRIDE;
			virtual Data*       GetValueData() const HELIUM_OVERRIDE;
			virtual void        GetItems( DataPointer association, DynamicArray<DataPointer>& keys, DynamicArray<DataPointer>& values ) HELIUM_OVERRIDE;
			virtual DataPointer GetItem( DataPointer association, DataPointer key ) HELIUM_OVERRIDE;
			virtual void        SetItem( DataPointer association, DataPointer key, DataPointer value ) HELIUM_OVERRIDE;
			virtual void        RemoveItem( DataPointer association, DataPointer key ) HELIUM_OVERRIDE;

        private:
            Data*               m_InternalDataKey;
            Data*               m_InternalDataValue;
		};
        
        template <class KeyT, class ValueT>
        inline Data* AllocateData( const Map<KeyT, ValueT>&, const Map<KeyT, ValueT>& )
        {
	        return new SimpleMapData<KeyT, ValueT>();
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