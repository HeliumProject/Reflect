#include "Precompile.h"
#include "Reflect/Registry.h"

#include "Platform/Atomic.h"
#include "Platform/Thread.h"

#include "Foundation/Log.h"

#include "Reflect/Object.h"
#include "Reflect/TranslatorDeduction.h"

// Prints the callstack for every init and cleanup call
// #define HELIUM_DEBUG_INIT_AND_CLEANUP

using namespace Helium;
using namespace Helium::Reflect;

// profile interface
#if HELIUM_PROFILE_ENABLE
Profile::Sink Reflect::g_CloneSink ( "Reflect Clone" );
Profile::Sink Reflect::g_ParseSink ( "Reflect Parse" );
Profile::Sink Reflect::g_AuthorSink ( "Reflect Author" );
Profile::Sink Reflect::g_ChecksumSink ( "Reflect Checksum" );
Profile::Sink Reflect::g_PreSerializeSink ( "Reflect Serialize Pre-Process" );
Profile::Sink Reflect::g_PostSerializeSink ( "Reflect Serialize Post-Process" );
Profile::Sink Reflect::g_PreDeserializeSink ( "Reflect Deserialize Pre-Process" );
Profile::Sink Reflect::g_PostDeserializeSink ( "Reflect Deserialize Post-Process" );
#endif

template< class T >
struct CaseInsensitiveCompare
{
    const std::string& value;

    CaseInsensitiveCompare( const std::string& str )
        : value( str )
    {

    }

    bool operator()( const std::pair< const std::string, T >& rhs )
    {
        return CaseInsensitiveCompare( rhs.first.c_str(), value.c_str() ) == 0;
    }
};

template< class T >
struct CaseInsensitiveNameCompare
{
    const char* value;

    CaseInsensitiveNameCompare( const char* name )
        : value( name )
    {

    }

    bool operator()( const KeyValue< const char*, T >& rhs )
    {
        return CaseInsensitiveCompare<T>( *rhs.First(), *value ) == 0;
    }
};

namespace Helium
{
    namespace Reflect
    {
        int32_t g_InitCount = 0;
        Registry* g_Registry = NULL;
    }
}

void Reflect::Startup()
{
    if (++g_InitCount == 1)
    {
        g_Registry = new Registry();

        MetaTypeRegistrar::RegisterTypes( RegistrarTypes::MetaEnum );
        MetaTypeRegistrar::RegisterTypes( RegistrarTypes::MetaStruct );
        MetaTypeRegistrar::RegisterTypes( RegistrarTypes::MetaClass );
    }

#ifdef HELIUM_DEBUG_INIT_AND_CLEANUP
    std::vector<uintptr_t> trace;
    Helium::GetStackTrace( trace );

    std::string str;
    Helium::TranslateStackTrace( trace, str );

    Log::Print( "\n" );
    Log::Print("%d\n\n%s\n", g_InitCount, str.c_str() );
#endif
}

void Reflect::Shutdown()
{
    if ( --g_InitCount == 0 )
    {
        MetaTypeRegistrar::UnregisterTypes( RegistrarTypes::MetaClass );
        MetaTypeRegistrar::UnregisterTypes( RegistrarTypes::MetaStruct );
        MetaTypeRegistrar::UnregisterTypes( RegistrarTypes::MetaEnum );

        delete g_Registry;
        g_Registry = NULL;
    }

#ifdef HELIUM_DEBUG_INIT_AND_CLEANUP
    std::vector<uintptr_t> trace;
    Helium::GetStackTrace( trace );

    std::string str;
    Helium::TranslateStackTrace( trace, str );

    Log::Print( "\n" );
    Log::Print("%d\n\n%s\n", g_InitCount, str.c_str() );
#endif
}

// private constructor
Registry::Registry()
{

}

Registry::~Registry()
{
    m_TypesByHash.Clear();
}

Registry* Registry::GetInstance()
{
    HELIUM_ASSERT(g_Registry != NULL);
    return g_Registry;
}

bool Registry::RegisterType(const MetaType* type)
{
    HELIUM_ASSERT( Thread::IsMain() );

    // hold a reference so it isn't deleted if its a redundant entry
    Helium::SmartPtr< MetaType > typeToRegister ( type );

    uint32_t crc = Crc32( type->m_Name );
    Pair< M_HashToType::Iterator, bool > result = m_TypesByHash.Insert( M_HashToType::ValueType( crc, type ) );
    if ( !HELIUM_VERIFY( result.Second() ) )
    {
        Log::Error( "Re-registration of type %s, could be ambigouous crc: 0x%08X\n", type->m_Name, crc );
        HELIUM_BREAK();
        return false;
    }

    type->Register();

    return true;
}

void Registry::UnregisterType(const MetaType* type)
{
    HELIUM_ASSERT( Thread::IsMain() );

    type->Unregister();

    uint32_t crc = Crc32( type->m_Name );
    m_TypesByHash.Remove( crc );
}

void Registry::AliasType( const MetaType* type, const char* alias )
{
    HELIUM_ASSERT( Thread::IsMain() );

    uint32_t crc = Crc32( alias );
    m_TypesByHash.Insert( M_HashToType::ValueType( crc, type ) );
}

void Registry::UnaliasType( const MetaType* type, const char* alias )
{
    HELIUM_ASSERT( Thread::IsMain() );

    uint32_t crc = Crc32( alias );
    M_HashToType::Iterator found = m_TypesByHash.Find( crc );
    if ( found != m_TypesByHash.End() && found->Second() == type )
    {
        m_TypesByHash.Remove( crc );
    }
}

const MetaType* Registry::GetType( uint32_t crc ) const
{
    M_HashToType::ConstIterator found = m_TypesByHash.Find( crc );

    if ( found != m_TypesByHash.End() )
    {
        return found->Second();
    }

    return NULL;
}

const MetaStruct* Registry::GetMetaStruct( uint32_t crc ) const
{
    return ReflectionCast< const MetaStruct >( GetType( crc ) );
}

const MetaClass* Registry::GetMetaClass( uint32_t crc ) const
{
    return ReflectionCast< const MetaClass >( GetType( crc ) );
}

const MetaEnum* Registry::GetMetaEnum( uint32_t crc ) const
{
    return ReflectionCast< const MetaEnum >( GetType( crc ) );
}
