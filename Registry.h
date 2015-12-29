#pragma once

#include "Platform/Types.h"

#include "Foundation/Crc32.h"
#include "Foundation/SortedMap.h"
#include "Foundation/FilePath.h"
#include "Foundation/SmartPtr.h"

#include "Reflect/Meta.h"

namespace Helium
{
    namespace Reflect
    {
        // Callbacks for external APIs
        typedef void (*CreatedFunc)(Object* object);
        typedef void (*DestroyedFunc)(Object* object);

        // Registry containers
        typedef SortedMap< uint32_t, Helium::SmartPtr< MetaType > > M_HashToType;

        // Profile interface
#if HELIUM_PROFILE_ENABLE
        extern Profile::Sink g_CloneSink;
        extern Profile::Sink g_ParseSink;
        extern Profile::Sink g_AuthorSink;
        extern Profile::Sink g_ChecksumSink;
        extern Profile::Sink g_PreSerializeSink;
        extern Profile::Sink g_PostSerializeSink;
        extern Profile::Sink g_PreDeserializeSink;
        extern Profile::Sink g_PostDeserializeSink;
#endif

        // Init/Cleanup
        HELIUM_REFLECT_API void Startup();
        HELIUM_REFLECT_API void Shutdown();

        class HELIUM_REFLECT_API Registry
        {
        private:
            friend void Reflect::Startup();
            friend void Reflect::Shutdown();

            Registry();
            ~Registry();

        public:
            // singleton constructor and accessor
            static Registry* GetInstance();

            // used for asserting on thread usage
            bool IsInitThread();

            // register type with registry with type id only
            bool RegisterType( const MetaType* type );
            void UnregisterType( const MetaType* type );

            // give a type an alias (for legacy considerations)
            void AliasType( const MetaType* type, const char* alias );
            void UnaliasType( const MetaType* type, const char* alias );

            // type lookup
            const MetaType* GetType( uint32_t crc ) const;
            inline const MetaType* GetType( const char* name ) const;
            
            // structure lookup
            const MetaStruct* GetMetaStruct( uint32_t crc ) const;
            inline const MetaStruct* GetMetaStruct( const char* name ) const;

            // class lookup
            const MetaClass* GetMetaClass( uint32_t crc ) const;
            inline const MetaClass* GetMetaClass( const char* name ) const;

            // enumeration lookup
            const MetaEnum* GetMetaEnum( uint32_t crc ) const;
            inline const MetaEnum* GetMetaEnum( const char* name ) const;

        private:
            M_HashToType        m_TypesByHash;
        };

        //
        // Helpers to resolve type information
        //

        template<class T>
        inline const MetaClass* GetMetaClass()
        {
            T::s_Registrar.Register();
            return T::s_MetaClass;
        }

        template<class T>
        inline const MetaStruct* GetMetaStruct()
        {
            T::s_Registrar.Register();
            return T::s_MetaStruct;
        }

        template<class T>
        inline const MetaEnum* GetMetaEnum()
        {
            T::s_Registrar.Register();
            return T::s_Enumeration;
        }
    }
}

#include "Reflect/Registry.inl"