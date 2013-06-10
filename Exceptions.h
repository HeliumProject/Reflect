#pragma once

#include "Platform/Exception.h"

namespace Helium
{
	namespace Reflect
	{
		//
		// Basic exception
		//

		class Exception : public Helium::Exception
		{
		public:
			Exception( const char *msgFormat, ... )
			{
				va_list msgArgs;
				va_start( msgArgs, msgFormat );
				SetMessage( msgFormat, msgArgs );
				va_end( msgArgs );
			}

		protected:
			Exception() throw() {} // hide default c_tor
		};

		//
		// For ThrowCast<>, etc...
		//

		class CastException : public Reflect::Exception
		{
		public:
			CastException( const char *msgFormat, ... )
			{
				va_list msgArgs;
				va_start( msgArgs, msgFormat );
				SetMessage( msgFormat, msgArgs );
				va_end( msgArgs );
			}

		protected:
			CastException() throw() {} // hide default c_tor
		};

		//
		// Reflection information is missing or invalid
		//

		class TypeInformationException : public Reflect::Exception
		{
		public:
			TypeInformationException( const char *msgFormat, ... )
			{
				va_list msgArgs;
				va_start( msgArgs, msgFormat );
				SetMessage( msgFormat, msgArgs );
				va_end( msgArgs );
			}

		protected:
			TypeInformationException() throw() {} // hide default c_tor
		};
	}
}