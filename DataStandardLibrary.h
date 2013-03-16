#pragma once

#include "Reflect/Registry.h"
#include "Reflect/DataSimple.h"

namespace Helium
{
	namespace Reflect
	{
#ifdef REFLECT_REFACTOR
		template <class T>
		class HELIUM_REFLECT_API SimpleStlVectorData : public SequenceData
		{
		public:
			typedef std::vector<T> DataType;
			DataPointer<DataType> m_Data;

			virtual void ConnectData(void* data) HELIUM_OVERRIDE;

			virtual size_t GetSize() const HELIUM_OVERRIDE;
			virtual void SetSize(size_t size) HELIUM_OVERRIDE;
			virtual void Clear() HELIUM_OVERRIDE;

			virtual const Class* GetItemClass() const HELIUM_OVERRIDE;
			virtual DataPtr GetItem(size_t at) HELIUM_OVERRIDE;
			virtual void SetItem(size_t at, Data* value) HELIUM_OVERRIDE;
			virtual void Insert( size_t at, Data* value ) HELIUM_OVERRIDE;
			virtual void Remove( size_t at ) HELIUM_OVERRIDE;
			virtual void MoveUp( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;
			virtual void MoveDown( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;

			virtual bool Set(Data* src, uint32_t flags = 0) HELIUM_OVERRIDE;
			virtual bool Equals(Object* object) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

			virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
			virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

			virtual tostream& operator>>(tostream& stream) const HELIUM_OVERRIDE;
			virtual tistream& operator<<(tistream& stream) HELIUM_OVERRIDE;
		};

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