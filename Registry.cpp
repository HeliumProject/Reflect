#include "ReflectPch.h"
#include "Reflect/Registry.h"

#include "Platform/Atomic.h"
#include "Platform/Thread.h"

#include "Foundation/Log.h"

#include "Reflect/Object.h"
#include "Reflect/TranslatorDeduction.h"

// Prints the callstack for every init and cleanup call
// #define REFLECT_DEBUG_INIT_AND_CLEANUP

using namespace Helium;
using namespace Helium::Reflect;

// profile interface
#ifdef PROFILE_ACCUMULATION
Profile::Accumulator Reflect::g_CloneAccum ( "Reflect Clone" );
Profile::Accumulator Reflect::g_ParseAccum ( "Reflect Parse" );
Profile::Accumulator Reflect::g_AuthorAccum ( "Reflect Author" );
Profile::Accumulator Reflect::g_ChecksumAccum ( "Reflect Checksum" );
Profile::Accumulator Reflect::g_PreSerializeAccum ( "Reflect Serialize Pre-Process" );
Profile::Accumulator Reflect::g_PostSerializeAccum ( "Reflect Serialize Post-Process" );
Profile::Accumulator Reflect::g_PreDeserializeAccum ( "Reflect Deserialize Pre-Process" );
Profile::Accumulator Reflect::g_PostDeserializeAccum ( "Reflect Deserialize Post-Process" );
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
        int32_t         g_InitCount = 0;
        Registry*   g_Registry = NULL;
    }
}

bool Reflect::IsInitialized()
{
    return g_Registry != NULL;
}

void Reflect::Initialize()
{
    if (++g_InitCount == 1)
    {
        g_Registry = new Registry();

        TypeRegistrar::RegisterTypes( RegistrarTypes::Enumeration );
        TypeRegistrar::RegisterTypes( RegistrarTypes::Structure );
        TypeRegistrar::RegisterTypes( RegistrarTypes::Object );
    }

#ifdef REFLECT_DEBUG_INIT_AND_CLEANUP
    std::vector<uintptr_t> trace;
    Helium::GetStackTrace( trace );

    std::string str;
    Helium::TranslateStackTrace( trace, str );

    Log::Print( "\n" );
    Log::Print("%d\n\n%s\n", g_InitCount, str.c_str() );
#endif
}

void Reflect::Cleanup()
{
    if ( --g_InitCount == 0 )
    {
        TypeRegistrar::UnregisterTypes( RegistrarTypes::Object );
        TypeRegistrar::UnregisterTypes( RegistrarTypes::Structure );
        TypeRegistrar::UnregisterTypes( RegistrarTypes::Enumeration );

        delete g_Registry;
        g_Registry = NULL;
    }

#ifdef REFLECT_DEBUG_INIT_AND_CLEANUP
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

bool Registry::RegisterType(const Type* type)
{
    HELIUM_ASSERT( IsMainThread() );

    uint32_t crc = Crc32( type->m_Name );
	Pair< M_HashToType::Iterator, bool > result = m_TypesByHash.Insert( M_HashToType::ValueType( crc, type ) );
    if ( !result.Second() )
    {
        Log::Error( TXT( "Re-registration of type %s, could be ambigouous crc: 0x%08x\n" ), type->m_Name, crc );
        HELIUM_BREAK();
        return false;
    }

    type->Register();

    return true;
}

void Registry::UnregisterType(const Type* type)
{
    HELIUM_ASSERT( IsMainThread() );

    type->Unregister();

    uint32_t crc = Crc32( type->m_Name );
    m_TypesByHash.Remove( crc );
}

void Registry::AliasType( const Type* type, const char* alias )
{
    HELIUM_ASSERT( IsMainThread() );

    uint32_t crc = Crc32( alias );
    m_TypesByHash.Insert( M_HashToType::ValueType( crc, type ) );
}

void Registry::UnaliasType( const Type* type, const char* alias )
{
    HELIUM_ASSERT( IsMainThread() );

    uint32_t crc = Crc32( alias );
    M_HashToType::Iterator found = m_TypesByHash.Find( crc );
    if ( found != m_TypesByHash.End() && found->Second() == type )
    {
        m_TypesByHash.Remove( crc );
    }
}

const Type* Registry::GetType( uint32_t crc ) const
{
    M_HashToType::ConstIterator found = m_TypesByHash.Find( crc );

    if ( found != m_TypesByHash.End() )
    {
        return found->Second();
    }

    return NULL;
}

const Structure* Registry::GetStructure( uint32_t crc ) const
{
    return ReflectionCast< const Structure >( GetType( crc ) );
}

const Class* Registry::GetClass( uint32_t crc ) const
{
    return ReflectionCast< const Class >( GetType( crc ) );
}

const Enumeration* Registry::GetEnumeration( uint32_t crc ) const
{
    return ReflectionCast< const Enumeration >( GetType( crc ) );
}
