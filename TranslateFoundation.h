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
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier ) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		inline const MetaType* DeduceKeyType( const String&, const String& )
		{
			return NULL;
		}
		inline const MetaType* DeduceValueType( const String&, const String& )
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
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier ) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		inline const MetaType* DeduceKeyType( const Name&, const Name& )
		{
			return NULL;
		}
		inline const MetaType* DeduceValueType( const Name&, const Name& )
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
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier ) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		inline const MetaType* DeduceKeyType( const FilePath&, const FilePath& )
		{
			return NULL;
		}
		inline const MetaType* DeduceValueType( const FilePath&, const FilePath& )
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
			virtual void        Construct( Pointer pointer ) override;
			virtual void        Destruct( Pointer pointer ) override;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) override;
			virtual bool        Equals( Pointer a, Pointer b ) override;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const override;
			virtual void        Clear( Pointer container ) override;

			// SequenceTranslator
			virtual Translator* GetItemTranslator() const override;
			virtual void        GetItems( Pointer sequence, DynamicArray< Pointer >& items ) const override;
			virtual void        SetLength( Pointer sequence, size_t length ) override;
			virtual Pointer     GetItem( Pointer sequence, size_t at ) override;
			virtual void        SetItem( Pointer sequence, size_t at, Pointer value ) override;
			virtual void        Insert( Pointer sequence, size_t at, Pointer value ) override;
			virtual void        Remove( Pointer sequence, size_t at ) override;
			virtual void        MoveUp( Pointer sequence, Set< size_t >& items ) override;
			virtual void        MoveDown( Pointer sequence, Set< size_t >& items ) override;

		private:
			void                SwapInternalValues(Pointer sequence, size_t a, size_t b);

			Translator*         m_InternalTranslator;
		};
		
		template <class T>
		inline const MetaType* DeduceKeyType( const DynamicArray<T>&, const DynamicArray<T>& )
		{
			return NULL;
		}
		template <class T>
		inline const MetaType* DeduceValueType( const DynamicArray<T>&, const DynamicArray<T>& )
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
			virtual void        Construct( Pointer pointer ) override;
			virtual void        Destruct( Pointer pointer ) override;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) override;
			virtual bool        Equals( Pointer a, Pointer b ) override;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const override;
			virtual void        Clear( Pointer container ) override;

			// SetTranslator
			virtual Translator* GetItemTranslator() const override;
			virtual void        GetItems( Pointer set, DynamicArray< Pointer >& items ) const override;
			virtual void        InsertItem( Pointer set, Pointer item ) override;
			virtual void        RemoveItem( Pointer set, Pointer item ) override;
			virtual bool        ContainsItem( Pointer set, Pointer item ) const override;

		private:
			Translator*         m_InternalTranslator;
		};
		
		template <class T>
		inline const MetaType* DeduceKeyType( const Set<T>&, const Set<T>& )
		{
			return NULL;
		}
		template <class T>
		inline const MetaType* DeduceValueType( const Set<T>&, const Set<T>& )
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
			virtual void              Construct( Pointer pointer ) override;
			virtual void              Destruct( Pointer pointer ) override;
			virtual void              Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) override;
			virtual bool              Equals( Pointer a, Pointer b ) override;

			// ContainerTranslator
			virtual size_t            GetLength( Pointer container ) const override;
			virtual void              Clear( Pointer container ) override;

			// AssociationTranslator
			virtual ScalarTranslator* GetKeyTranslator() const override;
			virtual Translator*       GetValueTranslator() const override;
			virtual void              GetItems( Pointer association, DynamicArray<Pointer>& keys, DynamicArray<Pointer>& values ) override;
			virtual Pointer           GetItem( Pointer association, Pointer key ) override;
			virtual void              SetItem( Pointer association, Pointer key, Pointer value ) override;
			virtual void              RemoveItem( Pointer association, Pointer key ) override;

		private:
			ScalarTranslator*   m_InternalTranslatorKey;
			Translator*         m_InternalTranslatorValue;
		};
		
		template <class KeyT, class ValueT>
		inline const MetaType* DeduceKeyType( const Map<KeyT, ValueT>&, const Map<KeyT, ValueT>& )
		{
			return DeduceValueType<KeyT>();
		}
		template <class KeyT, class ValueT>
		inline const MetaType* DeduceValueType( const Map<KeyT, ValueT>&, const Map<KeyT, ValueT>& )
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

#include "Reflect/TranslateFoundation.inl"
