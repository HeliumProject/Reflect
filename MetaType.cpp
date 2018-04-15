#include "Precompile.h"
#include "MetaType.h"

#include "Platform/Locks.h"
#include "Foundation/Log.h"
#include "Reflect/Object.h"
#include "Reflect/Registry.h"
#include "Reflect/Translator.h"

using namespace Helium::Reflect;

Helium::Mutex g_TypeMutex;

MetaType::MetaType()
: m_Tag( NULL )
, m_Name( NULL )
, m_Size( 0 )
{

}

MetaType::~MetaType()
{

}

void MetaType::Register() const
{
    const char* type = MetaIds::Strings[ GetMetaId() ];
    Log::Debug( "Reflect %s: 0x%" HELIUM_PRINT_POINTER ", Size: %4d, Name: %s (0x%08X)\n", type, this, m_Size, m_Name, Crc32( m_Name ) );
}

void MetaType::Unregister() const
{

}

MetaTypeRegistrar* MetaTypeRegistrar::s_Head[ RegistrarTypes::Count ] = { NULL, NULL, NULL };
MetaTypeRegistrar* MetaTypeRegistrar::s_Tail[ RegistrarTypes::Count ] = { NULL, NULL, NULL };

MetaTypeRegistrar::MetaTypeRegistrar( const char* name )
: m_Name( name )
, m_Next( NULL )
{

}

void MetaTypeRegistrar::AddToList( RegistrarType type, MetaTypeRegistrar* registrar )
{
    if ( s_Tail[ type ] )
    {
        s_Tail[ type ]->m_Next = registrar;
        s_Tail[ type ] = registrar;
    }
    else
    {
        s_Head[ type ] = registrar;
        s_Tail[ type ] = registrar;
    }
}

void MetaTypeRegistrar::RemoveFromList( RegistrarType type, MetaTypeRegistrar* registrar )
{
    if ( registrar == s_Head[ type ] )
    {
        s_Head[ type ] = s_Head[ type ]->m_Next;
    }
    else
    {
        for ( MetaTypeRegistrar *r = s_Head[ type ], *p = NULL; r; p = r, r = r->m_Next )
        {
            if ( r == registrar )
            {
                if ( p )
                {
                    p->m_Next = r->m_Next;
                }

                if ( r == s_Tail[ type ] )
                {
                    s_Tail[ type ] = NULL;
                }
            }
        }
    }
}

void MetaTypeRegistrar::RegisterTypes( RegistrarType type )
{
    for ( MetaTypeRegistrar* r = s_Head[ type ]; r; r = r->m_Next )
    {
        r->Register();
    }
}

void MetaTypeRegistrar::UnregisterTypes( RegistrarType type )
{
    for ( MetaTypeRegistrar* r = s_Head[ type ]; r; r = r->m_Next )
    {
        r->Unregister();
    }
}

void MetaTypeRegistrar::AddTypeToRegistry( const MetaType* type )
{
    Reflect::Registry::GetInstance()->RegisterType( type );
}

void MetaTypeRegistrar::RemoveTypeFromRegistry( const MetaType* type )
{
    Reflect::Registry::GetInstance()->UnregisterType( type );
}