#include "ReflectPch.h"
#include "Reflect/Tests.h"

using namespace Helium;
using namespace Reflect;

#if !HELIUM_RELEASE

REFLECT_DEFINE_ENUMERATION( TestEnumeration );

void TestEnumeration::EnumerateEnum( Enumeration& info )
{
	info.AddElement( ValueOne, TXT( "Value One" ) );
	info.AddElement( ValueTwo, TXT( "Value Two" ) );
}

REFLECT_DEFINE_BASE_STRUCTURE( TestStruct );

void TestStruct::PopulateStructure( Reflect::Structure& comp )
{
	comp.AddField( &TestStruct::m_Uint8,  "Unsigned 8-bit Integer" );
	comp.AddField( &TestStruct::m_Uint16, "Unsigned 16-bit Integer" );
	comp.AddField( &TestStruct::m_Uint32, "Unsigned 32-bit Integer" );
	comp.AddField( &TestStruct::m_Uint64, "Unsigned 64-bit Integer" );

	comp.AddField( &TestStruct::m_Int8,  "Signed 8-bit Integer" );
	comp.AddField( &TestStruct::m_Int16, "Signed 16-bit Integer" );
	comp.AddField( &TestStruct::m_Int32, "Signed 32-bit Integer" );
	comp.AddField( &TestStruct::m_Int64, "Signed 64-bit Integer" );

	comp.AddField( &TestStruct::m_Float32, "32-bit Floating Point" );
	comp.AddField( &TestStruct::m_Float64, "64-bit Floating Point" );
	
	comp.AddField( &TestStruct::m_StdVectorUint32, "std::vector of Signed 32-bit Integers" );
	comp.AddField( &TestStruct::m_StdSetUint32, "std::vector of Unsigned 32-bit Integers" );
	comp.AddField( &TestStruct::m_StdMapUint32, "std::map of Unsigned 32-bit Integers" );
	
	comp.AddField( &TestStruct::m_FoundationDynamicArrayUint32, "Dynamic Array of Signed 32-bit Integers" );
	comp.AddField( &TestStruct::m_FoundationSetUint32, "Set of Unsigned 32-bit Integers" );
	comp.AddField( &TestStruct::m_FoundationMapUint32, "Map of Unsigned 32-bit Integers" );
}

REFLECT_DEFINE_OBJECT( TestObject );

void TestObject::PopulateStructure( Reflect::Structure& comp )
{
	comp.AddField( &TestObject::m_Struct, "Structure" );
	comp.AddField( &TestObject::m_StructArray, "Structure Array" );

	comp.AddField( &TestObject::m_Enumeration, "Enumeration" );
	comp.AddField( &TestObject::m_EnumerationArray, "Enumeration Array" );
}

void Reflect::RunTests()
{
	StrongPtr< TestObject > object = new TestObject ();
}

#endif