#pragma once

#include "Reflect/Registry.h"
#include "Reflect/DataSimple.h"

namespace Helium
{
	namespace Reflect
	{
		// NOTE: Pretty much all of these members could be const provided necessary functions on m_InternalData are const
		template <class T>
		class SimpleStlVectorData : public SequenceData
		{
		public:
			SimpleStlVectorData();
			virtual ~SimpleStlVectorData();

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
		inline Data* AllocateData( const std::vector<T>&, const std::vector<T>& )
		{
			return new SimpleStlVectorData<T>();
		}

		//////////////////////////////////////////////////////////////////////////

		template <class T>
		class SimpleStlSetData : public SetData
		{
		public:
			SimpleStlSetData();
			virtual ~SimpleStlSetData();

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
		inline Data* AllocateData( const std::set<T>&, const std::set<T>& )
		{
			return new SimpleStlSetData<T>();
		}

#ifdef REFLECT_REFACTOR

		template <class DataT, class DataClassT>
		class HELIUM_REFLECT_API SimpleStlSetData : public SetData
		{
		public:
			typedef std::set<DataT> DataType;
			DataPointer<DataType> m_Data;

			virtual void ConnectData(void* data) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetItemClass() const HELIUM_OVERRIDE;
			virtual void GetItems(std::vector< DataPtr >& items) const HELIUM_OVERRIDE;
			virtual void AddItem(Data* value) HELIUM_OVERRIDE;
			virtual void RemoveItem(Data* value) HELIUM_OVERRIDE;
			virtual bool ContainsItem(Data* value) const HELIUM_OVERRIDE;

			virtual bool Set(Data* src, uint32_t flags = 0) HELIUM_OVERRIDE;
			virtual bool Equals(Object* object) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>(tostream& stream) const HELIUM_OVERRIDE;
			virtual tistream& operator<<(tistream& stream) HELIUM_OVERRIDE;

		private:
			template< class ArchiveT >
			void Serialize( ArchiveT& archive );
			template< class ArchiveT >
			void Deserialize( ArchiveT& archive );
		};

		template <class KeyT, class KeyClassT, class ValueT, class ValueClassT>
		class HELIUM_REFLECT_API SimpleStlMapData : public AssociationData
		{
		public:
			typedef std::map<KeyT, ValueT> DataType;
			DataPointer<DataType> m_Data;

			virtual void ConnectData(void* data) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetKeyClass() const HELIUM_OVERRIDE;
			virtual const Class* GetValueClass() const HELIUM_OVERRIDE;
			virtual void GetItems(V_ValueType& items) HELIUM_OVERRIDE;
			virtual DataPtr GetItem(Data* key) HELIUM_OVERRIDE;
			virtual void SetItem(Data* key, Data* value) HELIUM_OVERRIDE;
			virtual void RemoveItem(Data* key) HELIUM_OVERRIDE;

			virtual bool Set(Data* src, uint32_t flags = 0) HELIUM_OVERRIDE;
			virtual bool Equals(Object* object) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>(tostream& stream) const HELIUM_OVERRIDE;
			virtual tistream& operator<<(tistream& stream) HELIUM_OVERRIDE;

		private:
			template< class ArchiveT >
			void Serialize( ArchiveT& archive );
			template< class ArchiveT >
			void Deserialize( ArchiveT& archive );
		};
#endif 
	}
}

#include "Reflect/DataStandardLibrary.inl"