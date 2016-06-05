#pragma once

#include "Platform/Types.h"
#include "Platform/Assert.h"
#include "Platform/Utility.h"

#include "Foundation/Attribute.h"
#include "Foundation/ConcurrentHashSet.h"
#include "Foundation/Event.h"
#include "Foundation/FilePath.h"
#include "Foundation/ReferenceCounting.h"

#include "Reflect/API.h"
#include "Reflect/Exceptions.h"
#include "Reflect/MetaClass.h"

namespace Helium
{
	namespace Reflect
	{
		class Translator;

		class Object;
		typedef Helium::StrongPtr<Object> ObjectPtr;
		typedef Helium::StrongPtr<const Object> ConstObjectPtr;


		//
		// ObjectRefCountSupport provides the support interface for managing reference counting data
		//

		class HELIUM_REFLECT_API ObjectRefCountSupport
		{
		public:
			/// Base type of reference counted object.
			typedef Object BaseType;

			/// @name Object Destruction Support
			//@{
			inline static void PreDestroy( Object* pObject );
			inline static void Destroy( Object* pObject );
			//@}

			/// @name Reference Count Proxy Allocation Interface
			//@{
			static RefCountProxy< Object >* Allocate();
			static void Release( RefCountProxy< Object >* pProxy );

			static void Shutdown();
			//@}

#if HELIUM_ENABLE_MEMORY_TRACKING
			/// @name Active Proxy Iteration
			//@{
			static size_t GetActiveProxyCount();
			static bool GetFirstActiveProxy( ConcurrentHashSet< RefCountProxy< Object >* >::ConstAccessor& rAccessor );
			//@}
#endif

		private:
			struct StaticTranslator;

			/// Static proxy management data.
			static StaticTranslator* sm_pStaticTranslator;
		};

		//
		// Event delegate to support getting notified if this object changes
		//

		struct ObjectChangeArgs
		{
			const Object* m_Object;
			const Field* m_Field;

			ObjectChangeArgs( const Object* object, const Field* field = NULL )
				: m_Object( object )
				, m_Field( field )
			{
			}
		};
		typedef Helium::Signature< const ObjectChangeArgs&, Helium::AtomicRefCountBase > ObjectChangeSignature;

		//
		// Object is the abstract base class of a serializable class
		//

		class HELIUM_REFLECT_API Object : NonCopyable
		{
		protected:
			HELIUM_DECLARE_REF_COUNT( Object, ObjectRefCountSupport );

		protected:
			Object();

		public:
			virtual ~Object();

			//
			// Memory
			//

			void* operator new( size_t bytes );
			void* operator new( size_t bytes, void* memory );
			void operator delete( void* ptr, size_t bytes );
			void operator delete( void* ptr, void* memory );

			virtual void RefCountPreDestroy();
			virtual void RefCountDestroy();  // This should only be called by the reference counting system!
			virtual void RefCountSwapProxies( Object *pOtherObject );

			//
			// Type checking
			//

			// Reflection data
			static uint32_t s_DefaultPointerFlags;
			static const MetaClass* s_MetaClass;
			static MetaClassRegistrar< Object, void > s_Registrar;

			// Retrieves the reflection data for this instance
			virtual const Reflect::MetaClass* GetMetaClass() const;

			// Deduces type membership for this instance
			bool IsA( const Reflect::MetaClass* type ) const;

			// Create class data block for this type
			static const Reflect::MetaClass* CreateMetaClass();

			// Enumerates member data (stub)
			static void PopulateMetaType( Reflect::MetaClass& comp );

			//
			// Persistence
			//

			// Callbacks are executed at the appropriate time by the archive and cloning APIs
			virtual ObjectPtr GetTemplate() const;

			virtual void PreSerialize( const Field* field );
			virtual void PostSerialize( const Field* field );
			virtual void PreDeserialize( const Field* field );
			virtual void PostDeserialize( const Field* field );

			//
			// Utilities
			//

			// Do comparison logic against other object, checks type and field data
			virtual bool Equals( Object* object );

			// Copy this object's data into another object isntance
			virtual void CopyTo( Object* object );

			// Copy this object's data into a new instance
			virtual ObjectPtr Clone();

			//
			// Notification
			//

			// Event raised when an object is modified
			mutable ObjectChangeSignature::Event e_Changed;

			// Raise the modification event manually, null field mean ambiguous/multiple changes
			virtual void RaiseChanged( const Field* field = NULL ) const;

			// Notify a particular field was changed
			template< class FieldT >
			void FieldChanged( FieldT* fieldAddress ) const;

			// Modify and notify a field change
			template< class ObjectT, class FieldT >
			void ChangeField( FieldT ObjectT::* field, const FieldT& newValue );
		};

		//
		// AssertCast type checks in debug and asserts if failure, does no type checking in release
		//

		template<class DerivedT>
		inline DerivedT* AssertCast( Reflect::Object* base );

		template<class DerivedT>
		inline const DerivedT* AssertCast(const Reflect::Object* base);

		//
		// ThrowCast type checks and throws if failure
		//

		template<class DerivedT>
		inline DerivedT* ThrowCast(Reflect::Object* base);

		template<class DerivedT>
		inline const DerivedT* ThrowCast(const Reflect::Object* base);

		//
		// SafeCast always type checks and returns null if failure
		//

		template<class DerivedT>
		inline DerivedT* SafeCast(Reflect::Object* base);

		template<class DerivedT>
		inline const DerivedT* SafeCast(const Reflect::Object* base);


		//
		// Specifies an identifier for an object
		//

		class HELIUM_REFLECT_API ObjectIdentifier
		{
		public:
			virtual bool Identify( const ObjectPtr& object, Name* identity ) = 0;
		};

		//
		// Resolves an identifier to an object instance
		//

		class HELIUM_REFLECT_API ObjectResolver
		{
		public:
			virtual bool Resolve( const Name& identity, ObjectPtr& pointer, const MetaClass* pointerClass ) = 0;

			// helper to extract the class of the pointer
			template< class T > bool Resolve( const Name& identity, StrongPtr< T >& object );
		};
	}
}

#include "Reflect/Object.inl"