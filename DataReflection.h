#pragma once

#include "Reflect/Data.h"
#include "Reflect/Structure.h"

namespace Helium
{
	namespace Reflect
	{
		template< class T >
		class EnumerationData : public ScalarData
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::EnumerationData, EnumerationData, ScalarData );

			EnumerationData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual bool Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		template< class T >
		Data* AllocateData( const EnumerationBase&, T )
		{
			return new EnumerationData< T >;
		}

#ifdef REFLECT_REFACTOR
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
		Data* AllocateData( const StructureBase&, T )
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

#endif
	}
}

#include "Reflect/DataReflection.h"