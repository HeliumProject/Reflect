#pragma once

#include "Reflect/Registry.h"
#include "Reflect/Translators.h"

namespace Helium
{
	namespace Reflect
	{
		template <class T>
		class SimpleStlVectorTranslator : public SequenceTranslator
		{
		public:
			SimpleStlVectorTranslator();
			virtual ~SimpleStlVectorTranslator();

			// Translator
			virtual void        Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void        Accept( Pointer p, Visitor& visitor ) HELIUM_OVERRIDE;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( Pointer container ) HELIUM_OVERRIDE;

			// SequenceTranslator
			virtual Translator*       GetItemTranslator() const HELIUM_OVERRIDE;
			virtual void        GetItems( Pointer sequence, DynamicArray< Pointer >& items ) const HELIUM_OVERRIDE;
			virtual void        SetLength( Pointer sequence, size_t length ) HELIUM_OVERRIDE;
			virtual Pointer GetItem( Pointer sequence, size_t at ) HELIUM_OVERRIDE;
			virtual void        SetItem( Pointer sequence, size_t at, Pointer value ) HELIUM_OVERRIDE;
			virtual void        Insert( Pointer sequence, size_t at, Pointer value ) HELIUM_OVERRIDE;
			virtual void        Remove( Pointer sequence, size_t at ) HELIUM_OVERRIDE;
			virtual void        MoveUp( Pointer sequence, Set< size_t >& items ) HELIUM_OVERRIDE;
			virtual void        MoveDown( Pointer sequence, Set< size_t >& items ) HELIUM_OVERRIDE;

		private:
			void                SwapInternalValues(Pointer sequence, size_t a, size_t b);

			Translator*               m_InternalTranslator;
		};
		
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
			virtual void        Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void        Accept( Pointer p, Visitor& visitor ) HELIUM_OVERRIDE;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( Pointer container ) HELIUM_OVERRIDE;

			// SetTranslator
			virtual Translator*       GetItemTranslator() const HELIUM_OVERRIDE;
			virtual void        GetItems( Pointer set, DynamicArray< Pointer >& items ) const HELIUM_OVERRIDE;
			virtual void        InsertItem( Pointer set, Pointer item ) HELIUM_OVERRIDE;
			virtual void        RemoveItem( Pointer set, Pointer item ) HELIUM_OVERRIDE;
			virtual bool        ContainsItem( Pointer set, Pointer item ) const HELIUM_OVERRIDE;

		private:
			Translator*               m_InternalTranslator;
		};
		
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
			virtual void        Construct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Destruct( Pointer pointer ) HELIUM_OVERRIDE;
			virtual void        Copy( Pointer src, Pointer dest, uint32_t flags = 0 ) HELIUM_OVERRIDE;
			virtual bool        Equals( Pointer a, Pointer b ) HELIUM_OVERRIDE;
			virtual void        Accept( Pointer p, Visitor& visitor ) HELIUM_OVERRIDE;

			// ContainerTranslator
			virtual size_t      GetLength( Pointer container ) const HELIUM_OVERRIDE;
			virtual void        Clear( Pointer container ) HELIUM_OVERRIDE;

			// SetTranslator
			virtual Translator*       GetKeyTranslator() const HELIUM_OVERRIDE;
			virtual Translator*       GetValueTranslator() const HELIUM_OVERRIDE;
			virtual void        GetItems( Pointer association, DynamicArray<Pointer>& keys, DynamicArray<Pointer>& values ) HELIUM_OVERRIDE;
			virtual Pointer GetItem( Pointer association, Pointer key ) HELIUM_OVERRIDE;
			virtual void        SetItem( Pointer association, Pointer key, Pointer value ) HELIUM_OVERRIDE;
			virtual void        RemoveItem( Pointer association, Pointer key ) HELIUM_OVERRIDE;

		private:
			Translator*               m_InternalTranslatorKey;
			Translator*               m_InternalTranslatorValue;
		};
		
		template <class KeyT, class ValueT>
		inline Translator* AllocateTranslator( const std::map<KeyT, ValueT>&, const std::map<KeyT, ValueT>& )
		{
			return new SimpleStlMapTranslator<KeyT, ValueT>();
		}
	}
}

#include "Reflect/StandardLibrarySupport.inl"
