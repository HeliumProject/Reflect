#pragma once

#include "Foundation/DynamicArray.h"

#include "Reflect/MetaType.h"

namespace Helium
{
	namespace Reflect
	{
		//
		// Empty struct just for type deduction purposes
		//  don't worry though, even though this class is non-zero in size on its own,
		//  your derived struct type can use the memory this takes due to C/C++ standard
		//  'Empty Base Optimization'
		//
		struct HELIUM_REFLECT_API Enum
		{
		};

		class HELIUM_REFLECT_API MetaEnum : public MetaType
		{
		public:
			HELIUM_META_DERIVED( MetaIds::MetaEnum, MetaEnum, MetaType );

			class HELIUM_REFLECT_API Element
			{
			public:
				Element();
				Element( uint32_t value, const std::string& name, const std::string& helpText = "FIXME: SET THE HELP TEXT FOR THIS ENUMERATION ELEMENT" );

				uint32_t        m_Value;    // the value of the object
				std::string     m_Name;     // the name of the object
				std::string     m_HelpText; // the help text for the object
			};

			MetaEnum();
			~MetaEnum();

			template<class T>
			static void Create( MetaEnum const*& pointer, const char* name );

			virtual void Register() const override;
			virtual void Unregister() const override;

			void AddElement(uint32_t value, const std::string& name, const std::string& helpText = "FIXME: SET THE HELP TEXT FOR THIS ENUMERATION ELEMENT" );
			bool IsValid(uint32_t value) const;

			bool GetValue(const std::string& str, uint32_t& value) const;
			bool GetString(const uint32_t value, std::string& str) const;

			bool GetBitfieldValue(const std::vector< std::string >& strs, uint32_t& value) const;
			bool GetBitfieldStrings(const uint32_t value, std::vector< std::string >& strs) const;

			inline static bool IsFlagSet(uint32_t value, uint32_t flag);
			inline static void SetFlags(uint32_t& value, uint32_t flags);

			DynamicArray< MetaEnum::Element > m_Elements;
			bool                              m_IsBitfield;

		private:
			bool GetSingleValue(const std::string& str, uint32_t& value) const;
		};

		template< class EnumT >
		class MetaEnumRegistrar : public MetaTypeRegistrar
		{
		public:
			MetaEnumRegistrar(const char* name);
			~MetaEnumRegistrar();

			virtual void Register();
			virtual void Unregister();
		};
	}
}

// declares type checking functions
#define _REFLECT_DECLARE_ENUM( ENUMERATION ) \
public: \
Enum m_Value; \
ENUMERATION() : m_Value() {} \
ENUMERATION( const ENUMERATION& e ) : m_Value( e.m_Value ) {} \
ENUMERATION( const Enum& e ) : m_Value( e ) {} \
operator const Enum&() const { return m_Value; } \
static const Helium::Reflect::MetaEnum* CreateEnumeration(); \
static const Helium::Reflect::MetaEnum* s_Enumeration; \
static Helium::Reflect::MetaEnumRegistrar< ENUMERATION > s_Registrar;

// defines the static type info vars
#define _REFLECT_DEFINE_ENUM( ENUMERATION ) \
const Helium::Reflect::MetaEnum* ENUMERATION::CreateEnumeration() \
{ \
	HELIUM_ASSERT( s_Enumeration == NULL ); \
	Helium::Reflect::MetaEnum::Create< ENUMERATION >( s_Enumeration, #ENUMERATION ); \
	return s_Enumeration; \
} \
const Helium::Reflect::MetaEnum* ENUMERATION::s_Enumeration = NULL; \
Helium::Reflect::MetaEnumRegistrar< ENUMERATION > ENUMERATION::s_Registrar( #ENUMERATION );

// declares an enumeration
#define HELIUM_DECLARE_ENUM( ENUMERATION ) \
	_REFLECT_DECLARE_ENUM( ENUMERATION )

// defines an enumeration
#define HELIUM_DEFINE_ENUM( ENUMERATION ) \
	_REFLECT_DEFINE_ENUM( ENUMERATION )

#include "Reflect/MetaEnum.inl"