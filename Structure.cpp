#include "ReflectPch.h"
#include "Structure.h"
#include "Reflect/DataDeduction.h"

using namespace Helium::Reflect;

Structure::Structure ()
{
}

Structure::~Structure ()
{
    delete m_Default;
}

Structure* Structure::Create()
{
    return new Structure();
}
