#pragma once

#include <map>

#include "Platform/Types.h"

#include "Foundation/SmartPtr.h"

#include "Reflect/API.h"

//
// Obviously the reflection system itself can't use the same type checking as the code
//  its reflecting upon, so define a simple type checking system just for the reflection classes
//

#define REFLECTION_BASE(__ID, __Type) \
	typedef __Type This; \
	const static int s_ReflectionTypeID = __ID; \
	virtual int GetReflectionType() const { return __ID; } \
	virtual bool HasReflectionType(int id) const { return __ID == id; }

#define REFLECTION_TYPE(__ID, __Type, __Base) \
	typedef __Type This; \
	typedef __Base Base; \
	const static int s_ReflectionTypeID = __ID; \
	virtual int GetReflectionType() const HELIUM_OVERRIDE { return __ID; } \
	virtual bool HasReflectionType(int id) const HELIUM_OVERRIDE { return __ID == id || Base::HasReflectionType(id); }

namespace Helium
{
	namespace Reflect
	{
		//
		// All types have to belong to this enum
		//

		namespace ReflectionTypes
		{
			enum ReflectionType
			{
				Invalid = -1,

				// type meta-data class hierarchy
				Type,
					Enumeration,
					Structure,
						Class,
				
				// data abstraction class hierarchy
				Translator,
					ScalarTranslator,
					StructureTranslator,
					PointerTranslator,
					EnumerationTranslator,
					TypeTranslator,
					ContainerTranslator,
						SetTranslator,
						SequenceTranslator,
						AssociationTranslator,

				Count,
			};

			extern const char* Strings[ ReflectionTypes::Count ];
		}
		typedef ReflectionTypes::ReflectionType ReflectionType;

		//
		// A block of string-based properties
		//

		class HELIUM_REFLECT_API PropertyCollection
		{
		protected:
			mutable std::map< std::string, std::string > m_Properties;

		public:
			template<class T>
			inline void SetProperty( const std::string& key, const T& value ) const;

			template<class T>
			inline bool GetProperty( const std::string& key, T& value ) const;

			inline const std::string& GetProperty( const std::string& key ) const;
		};

		//
		// This lets us safely cast between reflection class pointers
		//

		class HELIUM_REFLECT_API ReflectionInfo : public Helium::AtomicRefCountBase< ReflectionInfo >, public PropertyCollection
		{
		public:
			REFLECTION_BASE( ReflectionTypes::Invalid, ReflectionInfo );

			ReflectionInfo();
			virtual ~ReflectionInfo();
		};

		template<typename T>
		T* ReflectionCast(ReflectionInfo* info)
		{
			return (info && info->HasReflectionType( T::s_ReflectionTypeID )) ? static_cast<T*>(info) : NULL;
		}

		template<typename T>
		const T* ReflectionCast(const ReflectionInfo* info)
		{
			return (info && info->HasReflectionType( T::s_ReflectionTypeID )) ? static_cast<const T*>(info) : NULL;
		}
	}
}

#include "Reflect/ReflectionInfo.inl"