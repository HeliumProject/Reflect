#include "ReflectPch.h"
#include "Reflect/DataDeduction.h"
#include "Reflect/Object.h"

using namespace Helium;
using namespace Reflect;

#if !HELIUM_RELEASE

struct TestStruct : Structure
{
	REFLECT_DECLARE_BASE_STRUCTURE( TestStruct );
	static void PopulateComposite( Reflect::Composite& comp );

	uint8_t  m_Uint8;
	uint16_t m_Uint16;
	uint32_t m_Uint32;
	uint64_t m_Uint64;

	int8_t  m_Int8;
	int16_t m_Int16;
	int32_t m_Int32;
	int64_t m_Int64;

	float32_t m_Float32;
	float64_t m_Float64;
};

REFLECT_DEFINE_BASE_STRUCTURE( TestStruct );

void TestStruct::PopulateComposite( Reflect::Composite& comp )
{
	comp.AddField( &TestStruct::m_Uint8,  "Unsigned 8-bit Integer" );
	comp.AddField( &TestStruct::m_Uint16, "Unsigned 16-bit Integer" );
	comp.AddField( &TestStruct::m_Uint32, "Unsigned 32-bit Integer" );
	comp.AddField( &TestStruct::m_Uint64, "Unsigned 64-bit Integer" );

	comp.AddField( &TestStruct::m_Int8,  "Signed 8-bit Integer" );
	comp.AddField( &TestStruct::m_Int16, "Signed 16-bit Integer" );
	comp.AddField( &TestStruct::m_Int32, "Signed 32-bit Integer" );
	comp.AddField( &TestStruct::m_Int64, "Signed 64-bit Integer" );

	comp.AddField( &TestStruct::m_Int32, "32-bit Floating Point" );
	comp.AddField( &TestStruct::m_Int64, "64-bit Floating Point" );
}

class TestObject : public Reflect::Object
{
public:
	REFLECT_DECLARE_OBJECT( TestObject, Reflect::Object );
	static void PopulateComposite( Reflect::Composite& comp );

private:
	TestStruct m_Struct;
	TestStruct m_StructArray[ 8 ];
};

REFLECT_DEFINE_OBJECT( TestObject );

void TestObject::PopulateComposite( Reflect::Composite& comp )
{
	comp.AddField( &TestObject::m_Struct, "Structure" );
	comp.AddField( &TestObject::m_StructArray, "Structure Array" );
}

void Func()
{
	AllocateData< uint8_t >();
	AllocateData< TestStruct >();
	AllocateData< ObjectPtr >();

	StrongPtr< TestObject > object = new TestObject ();
}

#endif