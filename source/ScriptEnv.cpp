#include "rendergraph/ScriptEnv.h"

#include <SM_Vector.h>

#include <chaiscript/chaiscript.hpp>

namespace rg
{

CU_SINGLETON_DEFINITION(ScriptEnv);

ScriptEnv::ScriptEnv()
{
    m_chai = std::make_unique<chaiscript::ChaiScript>();

    m_chai->add(
      chaiscript::fun<std::function<float()>>(
        []() {
            return static_cast<float>(rand()) / RAND_MAX;
        }), "rand");

    m_chai->add(
      chaiscript::fun<std::function<float(float f)>>(
        [](float f) {
            return abs(f);
        }), "abs");

    m_chai->add(chaiscript::user_type<sm::vec3>(), "vec3");
    m_chai->add(chaiscript::constructor<sm::vec3(float x, float y, float z)>(), "vec3");

    m_chai->add(chaiscript::user_type<sm::vec4>(), "vec4");
    m_chai->add(chaiscript::constructor<sm::vec4(float x, float y, float z, float w)>(), "vec4");

    m_chai->add(chaiscript::vector_conversion<std::vector<int>>());
    m_chai->add(chaiscript::vector_conversion<std::vector<sm::vec3>>());
    m_chai->add(chaiscript::vector_conversion<std::vector<sm::vec4>>());
}

}