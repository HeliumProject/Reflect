#pragma once

#include "Foundation/FilePath.h"
#include "Foundation/String.h"
#include "Foundation/Name.h"
#include "Foundation/Map.h"

#include "Reflect/Translator.h"

namespace Helium
{
	namespace Reflect
	{
		class HELIUM_REFLECT_API StringTranslator : public ScalarTranslator
		{
		public:
			StringTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier ) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline const Type* DeduceKeyType( const String&, const String& )
		{
			return NULL;
		}
		inline const Type* DeduceValueType( const String&, const String& )
		{
			return NULL;
		}
		inline Translator* AllocateTranslator( const String&, const String& )
		{
			return new StringTranslator;
		}

		//////////////////////////////////////////////////////////////////////////

		class HELIUM_REFLECT_API NameTranslator : public ScalarTranslator
		{
		public:
			NameTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier ) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline const Type* DeduceKeyType( const Name&, const Name& )
		{
			return NULL;
		}
		inline const Type* DeduceValueType( const Name&, const Name& )
		{
			return NULL;
		}
		inline Translator* AllocateTranslator( const Name&, const Name& )
		{
			return new NameTranslator;
		}

		//////////////////////////////////////////////////////////////////////////

		class HELIUM_REFLECT_API FilePathTranslator : public ScalarTranslator
		{
		public:
			FilePathTranslator();
			virtual void Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) HELIUM_OVERRIDE;
			virtual bool Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier ) HELIUM_OVERRIDE;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) HELIUM_OVERRIDE;
		};

		inline const Type* DeduceKeyType( const FilePath&, const FilePath& )
		{
			return NULL;
		}
		inline const Type* DeduceVauleType( const FilePath&, const FilePath& )
		{
			return NULL;
		}
		inline Translator* AllocateTranslator( const FilePath&, const FilePath& )
		{
			return new FilePathTranslator;
		}

		//////////////////////////////////////////////////////////////////////////

		template <class T>
		class SimpleDynamicArrayTranslator : public SequenceTranslator
		{
		public:
			SimpleDynamicArrayTranslator();
			virtual ~SimpleDynamicArrayTranslator();

			// Translator
			virtual void        Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( Pointer container ) HELIUM_OVERRIDE;

			// SequenceTranslator
			virtual Translator* GetItemTranslator() const HELIUM_OVERRIDE;
			virtual void        GetItems( Pointer sequence, DynamicArray< Pointer >& items ) const HELIUM_OVERRIDE;
			virtual void        SetLength( Pointer sequence, size_t length ) HELIUM_OVERRIDE;
			virtual Pointer     GetItem( Pointer sequence, size_t at ) HELIUM_OVERRIDE;
			virtual void        SetItem( Pointer sequence, size_t at, Pointer value ) HELIUM_OVERRIDE;
			virtual void        Insert( Pointer sequence, size_t at, Pointer value ) HELIUM_OVERRIDE;
			virtual void        Remove( Pointer sequence, size_t at ) HELIUM_OVERRIDE;
			virtual void        MoveUp( Pointer sequence, Set< size_t >& items ) HELIUM_OVERRIDE;
			virtual void        MoveDown( Pointer sequence, Set< size_t >& items ) HELIUM_OVERRIDE;

		private:
			void                SwapInternalValues(Pointer sequence, size_t a, size_t b);

			Translator*         m_InternalTranslator;
		};
		
		template <class T>
		inline const Type* DeduceKeyType( const DynamicArray<T>&, const DynamicArray<T>& )
		{
			return NULL;
		}
		template <class T>
		inline const Type* DeduceValueType( const DynamicArray<T>&, const DynamicArray<T>& )
		{
			return DeduceValueType<T>();
		}
		template <class T>
		inline Translator* AllocateTranslator( const DynamicArray<T>&, const DynamicArray<T>& )
		{
			return new SimpleDynamicArrayTranslator<T>();
		}

		//////////////////////////////////////////////////////////////////////////

		template <class T>
		class SimpleSetTranslator : public SetTranslator
		{
		public:
			SimpleSetTranslator();
			virtual ~SimpleSetTranslator();

			// Translator
			virtual void        Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( Pointer container ) HELIUM_OVERRIDE;

			// SetTranslator
			virtual Translator* GetItemTranslator() const HELIUM_OVERRIDE;
			virtual void        GetItems( Pointer set, DynamicArray< Pointer >& items ) const HELIUM_OVERRIDE;
			virtual void        InsertItem( Pointer set, Pointer item ) HELIUM_OVERRIDE;
			virtual void        RemoveItem( Pointer set, Pointer item ) HELIUM_OVERRIDE;
			virtual bool        ContainsItem( Pointer set, Pointer item ) const HELIUM_OVERRIDE;

		private:
			Translator*         m_InternalTranslator;
		};
		
		template <class T>
		inline const Type* DeduceKeyType( const Set<T>&, const Set<T>& )
		{
			return NULL;
		}
		template <class T>
		inline const Type* DeduceValueType( const Set<T>&, const Set<T>& )
		{
			return DeduceValueType<T>();
		}
		template <class T>
		inline Translator* AllocateTranslator( const Set<T>&, const Set<T>& )
		{
			return new SimpleSetTranslator<T>();
		}

		//////////////////////////////////////////////////////////////////////////

		template <class KeyT, class ValueT>
		class SimpleMapTranslator : public AssociationTranslator
		{
		public:
			SimpleMapTranslator();
			virtual ~SimpleMapTranslator();

			// Translator
			virtual void        Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( Pointer container ) HELIUM_OVERRIDE;

			// SetTranslator
			virtual Translator* GetKeyTranslator() const HELIUM_OVERRIDE;
			virtual Translator* GetValueTranslator() const HELIUM_OVERRIDE;
			virtual void        GetItems( Pointer association, DynamicArray<Pointer>& keys, DynamicArray<Pointer>& values ) HELIUM_OVERRIDE;
			virtual Pointer     GetItem( Pointer association, Pointer key ) HELIUM_OVERRIDE;
			virtual void        SetItem( Pointer association, Pointer key, Pointer value ) HELIUM_OVERRIDE;
			virtual void        RemoveItem( Pointer association, Pointer key ) HELIUM_OVERRIDE;

		private:
			Translator*         m_InternalTranslatorKey;
			Translator*         m_InternalTranslatorValue;
		};
		
		template <class KeyT, class ValueT>
		inline const Type* DeduceKeyType( const Map<KeyT, ValueT>&, const Map<KeyT, ValueT>& )
		{
			return DeduceValueType<KeyT>();
		}
		template <class KeyT, class ValueT>
		inline const Type* DeduceValueType( const Map<KeyT, ValueT>&, const Map<KeyT, ValueT>& )
		{
			return DeduceValueType<ValueT>();
		}
		template <class KeyT, class ValueT>
		inline Translator* AllocateTranslator( const Map<KeyT, ValueT>&, const Map<KeyT, ValueT>& )
		{
			return new SimpleMapTranslator<KeyT, ValueT>();
		}
	}
}

#include "Reflect/FoundationSupport.inl"
