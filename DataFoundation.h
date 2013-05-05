#pragma once

#include "Foundation/FilePath.h"
#include "Foundation/String.h"
#include "Foundation/Name.h"
#include "Foundation/Map.h"

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

		//////////////////////////////////////////////////////////////////////////

		class HELIUM_REFLECT_API NameData : public ScalarData
		{
		public:
			NameData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier ) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Data* AllocateData( const Name&, const Name& )
		{
			return new NameData;
		}

		//////////////////////////////////////////////////////////////////////////

		class HELIUM_REFLECT_API FilePathData : public ScalarData
		{
		public:
			FilePathData();
			virtual void Construct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( DataPointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( DataPointer src, DataPointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( DataPointer a, DataPointer b ) HELIUM_OVERRIDE;
			virtual void Accept( DataPointer pointer, Visitor& visitor ) HELIUM_OVERRIDE;
			virtual void Print( DataPointer pointer, String& string, ObjectIdentifier& identifier ) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, DataPointer pointer, ObjectResolver& resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline Data* AllocateData( const FilePath&, const FilePath& )
		{
			return new FilePathData;
		}

		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////

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
	}
}

#include "Reflect/DataFoundation.inl"
