#pragma once

#include "Reflect/TranslateBuiltin.h"

namespace Helium
{
	namespace Reflect
	{
		class HELIUM_REFLECT_API StlStringTranslator : public ScalarTranslator
		{
		public:
			StlStringTranslator();
			virtual void Construct( Pointer pointer ) override;
			virtual void Destruct( Pointer pointer ) override;
			virtual void Copy( Pointer src, Pointer dest, uint32_t flags ) override;
			virtual bool Equals( Pointer a, Pointer b ) override;
			virtual void Print( Pointer pointer, String& string, ObjectIdentifier* identifier ) override;
			virtual void Parse( const String& string, Pointer pointer, ObjectResolver* resolver, bool raiseChanged ) override;
		};

		inline const MetaType* DeduceKeyType( const std::string&, const std::string& )
		{
			return NULL;
		}
		inline const MetaType* DeduceValueType( const std::string&, const std::string& )
		{
			return NULL;
		}
		inline Translator* AllocateTranslator( const std::string&, const std::string& )
		{
			return new StlStringTranslator;
		}

		template <class T>
		class SimpleStlVectorTranslator : public SequenceTranslator
		{
		public:
			SimpleStlVectorTranslator();
			virtual ~SimpleStlVectorTranslator();

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
			virtual Pointer GetItem( Pointer sequence, size_t at ) override;
			virtual void        SetItem( Pointer sequence, size_t at, Pointer value ) override;
			virtual void        Insert( Pointer sequence, size_t at, Pointer value ) override;
			virtual void        Remove( Pointer sequence, size_t at ) override;
			virtual void        MoveUp( Pointer sequence, Set< size_t >& items ) override;
			virtual void        MoveDown( Pointer sequence, Set< size_t >& items ) override;

		private:
			void                SwapInternalValues(Pointer sequence, size_t a, size_t b);

			Translator* m_InternalTranslator;
		};
		
		template <class T>
		inline const MetaType* DeduceKeyType( const std::vector<T>&, const std::vector<T>& )
		{
			return NULL;
		}
		template <class T>
		inline const MetaType* DeduceValueType( const std::vector<T>&, const std::vector<T>& )
		{
			return DeduceValueType<T>();
		}
		template <class T>
		inline Translator* AllocateTranslator( const std::vector<T>&, const std::vector<T>& )
		{
			return new SimpleStlVectorTranslator<T>();
		}

		//////////////////////////////////////////////////////////////////////////

		template <class T>
		class SimpleStlSetTranslator : public SetTranslator
		{
		public:
			SimpleStlSetTranslator();
			virtual ~SimpleStlSetTranslator();

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
			Translator* m_InternalTranslator;
		};
		
		template <class T>
		inline const MetaType* DeduceKeyType( const std::set<T>&, const std::set<T>& )
		{
			return NULL;
		}
		template <class T>
		inline const MetaType* DeduceValueType( const std::set<T>&, const std::set<T>& )
		{
			return DeduceValueType<T>();
		}
		template <class T>
		inline Translator* AllocateTranslator( const std::set<T>&, const std::set<T>& )
		{
			return new SimpleStlSetTranslator<T>();
		}

		//////////////////////////////////////////////////////////////////////////

		template <class KeyT, class ValueT>
		class SimpleStlMapTranslator : public AssociationTranslator
		{
		public:
			SimpleStlMapTranslator();
			virtual ~SimpleStlMapTranslator();

			// Translator
			virtual void              Construct( Pointer pointer ) override;
			virtual void              Destruct( Pointer pointer ) override;
			virtual void              Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) override;
			virtual bool              Equals( Pointer a, Pointer b ) override;

			// ContainerTranslator
			virtual size_t            GetLength( Pointer container ) const override;
			virtual void              Clear( Pointer container ) override;

			// AssocationTranslator
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
		inline const MetaType* DeduceKeyType( const std::map<KeyT, ValueT>&, const std::map<KeyT, ValueT>& )
		{
			return DeduceValueType<KeyT>();
		}
		template <class KeyT, class ValueT>
		inline const MetaType* DeduceValueType( const std::map<KeyT, ValueT>&, const std::map<KeyT, ValueT>& )
		{
			return DeduceValueType<ValueT>();
		}
		template <class KeyT, class ValueT>
		inline Translator* AllocateTranslator( const std::map<KeyT, ValueT>&, const std::map<KeyT, ValueT>& )
		{
			return new SimpleStlMapTranslator<KeyT, ValueT>();
		}
	}
}

#include "Reflect/TranslateStandardLibrary.inl"
