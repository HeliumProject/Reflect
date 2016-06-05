#pragma once

#include <vector>
#include <algorithm>

#include "Platform/Assert.h"
#include "Platform/Exception.h"
#include "Foundation/SmartPtr.h"

#include "API.h"
#include "Meta.h"

namespace Helium
{
    namespace Reflect
    {
        //
        // MetaType, fully qualified type information
        //

        class HELIUM_REFLECT_API MetaType : public Meta
        {
        public:
            HELIUM_META_BASE( MetaIds::MetaType, MetaType );

        protected:
            MetaType();
            ~MetaType();

        public:
            mutable const void*             m_Tag;          // tag (client) data
            const char*                  m_Name;         // the name of this type in the codebase
            uint32_t                        m_Size;         // the size of the object in bytes

            virtual void Register() const;
            virtual void Unregister() const;
        };

        namespace RegistrarTypes
        {
            enum RegistrarType
            {
                MetaEnum,
                MetaStruct,
                MetaClass,
                Count,
            };
        }
        typedef RegistrarTypes::RegistrarType RegistrarType;

        class HELIUM_REFLECT_API MetaTypeRegistrar
        {
        public:
            MetaTypeRegistrar(const char* name);

            virtual void Register() = 0;
            virtual void Unregister() = 0;

            static void AddToList( RegistrarType type, MetaTypeRegistrar* registrar );
            static void RemoveFromList( RegistrarType type, MetaTypeRegistrar* registrar );

            static void RegisterTypes( RegistrarType type );
            static void UnregisterTypes( RegistrarType type );

            static void AddTypeToRegistry( const MetaType* type );
            static void RemoveTypeFromRegistry( const MetaType* type );

        private:
            const char*               m_Name;
            MetaTypeRegistrar*        m_Next;
            static MetaTypeRegistrar* s_Head[ RegistrarTypes::Count ];
            static MetaTypeRegistrar* s_Tail[ RegistrarTypes::Count ];
        };
    }
}