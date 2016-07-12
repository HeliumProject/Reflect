<a href="http://heliumproject.org/">![Helium Game Engine](https://raw.githubusercontent.com/HeliumProject/Helium/master/Editor/Icons/Helium/Helium.png)</a>

Reflect is an implementation of C++ reflection (also known as introspection).  Reflection adds the ability for a C++ program to know about its own topology and coding factors.  This information can in turn be used to automate what would otherwise be rote programming tasks.  Removing human elements from rote programming tasks can lower the cost of adding new features to an application, and can reduce the number of moving parts that need modification by an engineer (which reduces the number of bugs that can be introduced by any given feature).

At the time of this writing the big ticket automations that build on Reflect are object persistence using [Persist](https://github.com/HeliumProject/Persist) and user interface generation using [Inspect](https://github.com/HeliumProject/Inspect)

History
=======

Reflect was initially implemented and was a cornerstone technology for asset persistence within the tools pipeline at Insomniac Games between 2004-2009.  It was grown from the ground up starting with basic type and member metadata implemented by [lpetre](https://github.com/lpetre), and was extended and refactored years thereafter by [gorlak](https://github.com/gorlak), [andyburke](https://github.com/andyburke), [kramdar](https://github.com/kramdar), and others to include:
* Thread-safe reference counting
* Type checking with asserting and throwing casting functions (replacement for dynamic_cast<>)
* Factory allocation with default value extraction and comparison
* Persistence of pointers to nested objects as well as containers of POD and object types
* Support for string representation of enumerations and bitfields

After Insomniac Games turned over their entire tech stack in 2010, Reflect continued development in the public domain:
* Type agnostic container introspection and modification (without instantiating template code of the container)
* Support or C-arrays of POD, struct and object types
* Support for direct aggregate structs in objects
* Cleaner separation of support for Helium's standard library and the C++ standard library
* Shift from manually printed XML generation to document-style libraries (JSON via rapidjson and MongoDB's BSON via mongo-c)

Design
======

Reflect is designed to be completely code-driven.  Reflect doesn't require any tool besides your compiler parse your source code.  Reflect generates C++ reflection metadata purely by code in your application.  There is no data to load at runtime, and because of this it avoid tricky issues involving synchronizing metadata version against the current executable version.  Classes, structures, and enumerations must be instrumented to add any cost (memory to store metadata) to your program.  Linking code change to runtime cost is a subtle but important distinction.  Historically, many high-performance projects disable built-in RTTI type checking since it adds cost for every class that has any virtual functions.  By being explicit about which classes increase metadata memory footprint Reflect allows engineers to be vigilant about how they consume resources on their target machine.

Reflect was designed to not require a lot of hand-written code to have metadata be generated for your application code.  It typically requires (per class, struct, or enum):
* A header file macro
* A source file macro
* A declaration of a static function to populate metadata (optional)
* A definition of that static function (optional)

For C++ classes the macros expose your class to the reflection system and adds type checking (similar to dynamic_cast<>), and the function only needs to be implemented for classes that have elements they want to expose (which in practice will be most of them).

Reflect is '''not''' designed to support the entire complexity of data structure layout of C++.  However it does support a wide range of common mechanics like:
* scalars (including pointers to certain types of objects)
* structures (simple aggregated structures)
* containers of:
** scalars (including pointers to certain types of objects)
** structures
** nested containers
* single dimensional C-style arrays of:
** scalars (including pointers to certain types of objects)
** structures
** containers

Usage
-----

```C++
//
// Enum
//

struct HeroType : Helium::Reflect::Enum
{
	enum Enum
	{
		Alien,
		Mutant,
		God,
	};

	HELIUM_DECLARE_ENUM( HeroType );
	static void PopulateMetaType( MetaEnum& info );
};

HELIUM_DEFINE_ENUM( HeroType );

void HeroType::PopulateMetaType( MetaEnum& info )
{
	info.AddElement( Alien, "alien" );
	info.AddElement( Mutant, "mutant" );
	info.AddElement( God, "god" );
}

//
// Struct (aggregated inside containers or objects)
//

struct Hero : Helium::Reflect::Struct
{
	Helium::String m_Name;
	HeroType       m_HeroType;
	uint32_t       m_HitPoints;

	HELIUM_DECLARE_BASE_STRUCT( Hero );
	static void PopulateMetaType( MetaStruct& comp );
};

HELIUM_DEFINE_BASE_STRUCT( Hero );

void Hero::PopulateMetaType( Reflect::MetaStruct& comp )
{
	comp.AddField( &Hero::m_Name, "name" );
	comp.AddField( &Hero::m_HeroType, "type" );
	comp.AddField( &Hero::m_HitPoints, "hp" );
}

//
// Object (heap allocated, type-checked, reference-counted)
//

class ComicUniverse : public Helium::Reflect::Object
{
	Hero m_Heroes[ 8 ];

	HELIUM_DECLARE_CLASS( ComicUniverse, Object );
	static void PopulateMetaType( MetaClass& comp );
};

HELIUM_DEFINE_CLASS( ComicUniverse );

void ComicUniverse::PopulateMetaType( Reflect::MetaClass& comp )
{
	comp.AddField( &ComicUniverse::m_Heroes, "heroes" );
}

```

Style
-----

Reflect expands on the distinction between the 'class' and 'struct' keyword in C++.  C++ is a big language, and there are many ways to use composition-style user defined types.  However, two disinct patterns emerge in most C++ codebases.  Reflect draws a clear distinction between these two patterns to balance keeping the reflection system lean while at the same time supporting many complex data structure organizations:

'class'
* Derive from Reflect::Object
* Have virtual functions (and a virtual destructor)
* Are heap allocated (and cleaned up via a thread-safe reference counting system)

'struct'
* Have no inheritance restriction (types can be a base struct, or inherit from other structs)
* Do not require virtual functions (but can have them if the user desires)
* Are allocated inside a 'class'-style type (aggregated directly, inside a container, or via C-style array)

Given these tradeoffs, many APIs just deal with 'class' types as the first-class citizen and 'struct' types as helper types.  An example of this is how Persist is designed to only serialize a Reflect::Object* and not a void*.

Metadata
--------

Many metadata class has additional flags that can be populated to inform the behavior of automation, for example FieldFlags ([here](https://github.com/HeliumProject/Reflect/blob/master/MetaStruct.h) can tell automation to deep copy or shallow copy pointer fields.

Beyond this Reflect allows for decoration of user metadata via a string-based property system (key-value pairs).  All of the metadata classes inherit from PropertyCollection ([here](https://github.com/HeliumProject/Reflect/blob/master/ReflectionInfo.h)), thus can easily be adorned with additional information in your enumeration function.

Implementation
==============

Registry
--------

The Reflect::Registy catalogs all the types registered to Reflect.  If one wants the metadata for a type knowing just the fully-qualified name or name CRC-32, they can ask the Registry for that metadata.

Types don't need to be manually registered, there are helper Reflect::Registrat objects (globals with constructors and destructors) that are instantiated inside the type macros.  While global constructors are typically deemed a dangerous method, these Registrar objects merely build a static linked list of Registrar objects (instead of allocating any heap memory before main(), and do not require freeing after main() has returned).  This was deemed a reasonable trade-off for avoiding manual registration (and release) of every type of object in your codebase, an onerous task.

Translators
-----------

Reflect::MetaStruct (and hence Reflect::MetaClass) is basically an array of Reflect::Fields.  Field is a concrete class that contains the metadata for a member variable in a 'class' or 'struct' exposed to Reflect.  Field doesn't have the knowledge of how to transact with the field it describes.  All read and write work done to any given field is instead done by a Translator.  The Reflect Translator classes abstract type-specific operations on an instance of a field within an instance of a reflected type.

Every Field heap allocates a Translator instance when metadata is generated.  There is a Translator instance for every field of every type that remains allocated for the life of the metadata (and hence probably the life of your program).  Any time some code wants to interact with the data stored in a Field they use the Translator object allocated for that Field.  Allocating a separate Translator object for every Field was a thoughtful decision, and while it does create many heap allocations on startup, it avoids a lot of code complexity to share Translator instances between Fields (because identifying that any two fields are of exact synonomous type as well as having support for type-tracking all the different types of Translator classes create an enormous amount of complexity).

Translator classes are polymorphic to support different topologies of data:
* ScalarTranslator provides an interface to any data that can be represented as a simple string (such as POD and pointer data)
* SetTranslator provides an interface for set containers (std::set<>, Helium::Set<>, Helium::HashSet<>)
* SequenceTranslator provides an interface dynamic array containers (std::vector<>, Helium::DynamicArray<>)
* AssociationTranslator provides an interface associative array containers (std::map<>, Helium::Map<>, Helium::Hashtable<>)

Translator classes also separate custom handling for different types of data:
* PointerTranslator handles dealing with object references
* TypeTranslator handles dealing with identifying types

Since Translator classes are polymorphic they support read and write operations to containers without having to be coded to the exact type of the data stored in the container.  Put another way, you can interface with template instantiations from non-template type-agnostic code.

Location
========
https://github.com/HeliumProject/Reflect
