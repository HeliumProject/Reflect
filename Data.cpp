#include "ReflectPch.h"
#include "Reflect/Data.h"

#include "Reflect/Composite.h"

using namespace Helium;
using namespace Helium::Reflect;

void DataInstance::RaiseChanged( bool doIt ) const
{
	if ( doIt )
	{
		HELIUM_ASSERT( m_Instance );
		HELIUM_ASSERT( m_Field );
		if ( m_Field->m_Composite->GetReflectionType() == ReflectionTypes::Class )
		{
			Object* object = static_cast< Object* >( m_Instance );
			object->RaiseChanged( m_Field );
		}
	}
}
