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

//#define HELIUM_PROFILE

#if defined(PROFILE_INSTRUMENT_ALL) || defined(REFLECT_PROFILE)
#define HELIUM_REFLECT_SCOPE_TIMER(__Str) PROFILE_SCOPE_TIMER(__Str)
#else
#define HELIUM_REFLECT_SCOPE_TIMER(__Str)
#endif

#if !HELIUM_RELEASE
# define HELIUM_REFLECT_CHECK_MEMORY 1
# define HELIUM_REFLECT_CHECK_MEMORY_ASSERT HELIUM_ASSERT
#else
# define HELIUM_REFLECT_CHECK_MEMORY_ASSERT( EXPR )
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
