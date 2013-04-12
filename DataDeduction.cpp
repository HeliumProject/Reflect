#include "ReflectPch.h"
#include "Reflect/DataDeduction.h"

using namespace Helium;
using namespace Reflect;

struct TestStruct : Structure
{
};

void Func()
{
	AllocateData< uint8_t >();
	AllocateData< TestStruct >();
	AllocateData< ObjectPtr >();
}