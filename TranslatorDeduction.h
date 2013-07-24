#pragma once

#include "Reflect/Translator.h"
#include "Reflect/TranslateBuiltin.h"
#include "Reflect/TranslateFoundation.h"
#include "Reflect/TranslateStandardLibrary.h"

namespace Helium
{
	namespace Reflect
	{
		template< class T >
		const MetaType* DeduceKeyType()
		{
			T t = T ();
			return DeduceKeyType( t, t );
		}

		template< class T >
		const MetaType* DeduceValueType()
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