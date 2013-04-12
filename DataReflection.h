#pragma once

#include "Reflect/Data.h"
#include "Reflect/Composite.h"

namespace Helium
{
	namespace Reflect
	{
#ifdef REFLECT_REFACTOR
		class HELIUM_REFLECT_API TypeIDData : public Data
		{
		public:
			typedef TypeID DataType;
			DataPointer<DataType> m_Data;

			virtual void ConnectData(void* data) HELIUM_OVERRIDE;

			virtual bool Set(Data* data, uint32_t flags = 0) HELIUM_OVERRIDE;
			virtual bool Equals(Object* object) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveBinary& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveBinary& archive) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveXML& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveXML& archive) HELIUM_OVERRIDE;
		};

		class HELIUM_REFLECT_API EnumerationData : public Data
		{
		public:
			typedef uint32_t DataType;
			DataPointer<DataType> m_Data;
			tstring m_String;

			virtual void ConnectData(void* data) HELIUM_OVERRIDE;

			virtual bool Set(Data* src, uint32_t flags = 0) HELIUM_OVERRIDE;
			virtual bool Equals(Object* object) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveBinary& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveBinary& archive) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveXML& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveXML& archive) HELIUM_OVERRIDE;

			virtual tostream& operator>>(tostream& stream) const HELIUM_OVERRIDE;
			virtual tistream& operator<<(tistream& stream) HELIUM_OVERRIDE;
		};

		class HELIUM_REFLECT_API BitfieldData : public EnumerationData
		{
		public:
			virtual void Serialize(ArchiveBinary& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveBinary& archive) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveXML& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveXML& archive) HELIUM_OVERRIDE;

			virtual tostream& operator>>(tostream& stream) const;
			virtual tistream& operator<<(tistream& stream);
		};

		class HELIUM_REFLECT_API PointerData : public Data
		{
		public:
			typedef ObjectPtr DataType;
			DataPointer<DataType> m_Data;

			PointerData ();
			~PointerData();

			virtual void ConnectData(void* data) HELIUM_OVERRIDE;

			virtual bool Set(Data* data, uint32_t flags = 0) HELIUM_OVERRIDE;
			virtual bool Equals(Object* object) HELIUM_OVERRIDE;
			virtual void Accept(Visitor& visitor) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveBinary& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveBinary& archive) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveXML& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveXML& archive) HELIUM_OVERRIDE;

			uint32_t GetLinkIndex() { return m_Data->GetLinkIndex(); }
			void ClearLinkIndex() { m_Data->ClearLinkIndex(); }

		private:
			template< class ArchiveT >
			void Serialize(ArchiveT& archive);
			template< class ArchiveT >
			void Deserialize(ArchiveT& archive);
		};
#endif

		template< class T >
		Data* AllocateData( const StrongPtr< T >&, const StrongPtr< T >& )
		{
#ifdef REFLECT_REFACTOR
			PointerData* data = new PointerData;
			data->m_Class = T::s_Class;
			HELIUM_ASSERT( data->m_Class );
			return data;
#else
			HELIUM_ASSERT( false );
			return NULL;
#endif
		}

#ifdef REFLECT_REFACTOR
		class HELIUM_REFLECT_API StructureData : public Data
		{
		public:
			typedef void DataType;
			VoidDataPointer m_Data;

			virtual void ConnectData(void* data) HELIUM_OVERRIDE;

			virtual bool Set(Data* data, uint32_t flags = 0) HELIUM_OVERRIDE;
			virtual bool Equals(Object* object) HELIUM_OVERRIDE;
			virtual void Accept(Visitor& visitor) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveBinary& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveBinary& archive) HELIUM_OVERRIDE;

			virtual void Serialize(ArchiveXML& archive) HELIUM_OVERRIDE;
			virtual void Deserialize(ArchiveXML& archive) HELIUM_OVERRIDE;

			void AllocateForArrayEntry(void *instance, const Field *field);
		};
#endif

		template< class T >
		Data* AllocateData( const Structure&, T )
		{
#ifdef REFLECT_REFACTOR
			StructureData* data = new StructureData;
			data->m_Structure = T::s_Structure;
			HELIUM_ASSERT( data->m_Structure );
			return data;
#else
			HELIUM_ASSERT( false );
			return NULL;
#endif
		}
	}
}