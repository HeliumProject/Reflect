#pragma once

#include "Reflect/Translator.h"
#include "Reflect/Translators.h"
#include "Reflect/FoundationSupport.h"
#include "Reflect/StandardLibrarySupport.h"

namespace Helium
{
	namespace Reflect
	{
		template< class T >
		const Type* DeduceKeyType()
		{
			T t = T ();
			return DeduceKeyType( t, t );
		}

		template< class T >
		const Type* DeduceValueType()
		{
			T t = T ();
			return DeduceValueType( t, t );
		}

		template< class T >
		Translator* AllocateTranslator()
		{
			T t = T ();
			return AllocateTranslator( t, t );
		}		
	}
}