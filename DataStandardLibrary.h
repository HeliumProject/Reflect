#pragma once

#include "Reflect/Registry.h"
#include "Reflect/DataSimple.h"

namespace Helium
{
	namespace Reflect
	{
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

		//////////////////////////////////////////////////////////////////////////

		template <class KeyT, class ValueT>
		class SimpleStlMapData : public AssociationData
		{
		public:
			SimpleStlMapData();
			virtual ~SimpleStlMapData();

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
		inline Data* AllocateData( const std::map<KeyT, ValueT>&, const std::map<KeyT, ValueT>& )
		{
			return new SimpleStlMapData<KeyT, ValueT>();
		}
	}
}

#include "Reflect/DataStandardLibrary.inl"
