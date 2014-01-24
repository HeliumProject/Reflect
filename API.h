#pragma once

#include "Platform/System.h"

#include "Foundation/Profile.h"
#include "Foundation/ReferenceCounting.h"

#if HELIUM_SHARED
# ifdef HELIUM_REFLECT_EXPORTS
#  define HELIUM_REFLECT_API HELIUM_API_EXPORT
# else
#  define HELIUM_REFLECT_API HELIUM_API_IMPORT
# endif
#else
#define HELIUM_REFLECT_API
#endif

#define HELIUM_REFLECT_PROFILE 0

#if HELIUM_PROFILE_INSTRUMENT_ALL || HELIUM_REFLECT_PROFILE
#define HELIUM_REFLECT_SCOPE_TIMER( ... ) HELIUM_PROFILE_SCOPE_TIMER( __VA_ARGS__ )
#else
#define HELIUM_REFLECT_SCOPE_TIMER( ... )
#endif

namespace Helium
{
	namespace Reflect
	{
		class MetaType;
		class MetaStruct;
		class MetaClass;
		class MetaEnum;
		class Translator;
		class Object;
	}
}
