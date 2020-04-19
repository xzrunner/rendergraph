#include "rendergraph/ScriptEnv.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/node/Shader.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <sm_const.h>

#include <chaiscript/chaiscript.hpp>

namespace rendergraph
{

CU_SINGLETON_DEFINITION(ScriptEnv);

ScriptEnv::ScriptEnv()
{
    m_chai = std::make_unique<chaiscript::ChaiScript>();

    // math
    m_chai->add(chaiscript::user_type<sm::vec2>(), "vec2");
    m_chai->add(chaiscript::constructor<sm::vec2(float x, float y)>(), "vec2");
    m_chai->add(chaiscript::fun(&sm::vec2::operator=), "=");

    m_chai->add(chaiscript::user_type<sm::vec3>(), "vec3");
    m_chai->add(chaiscript::constructor<sm::vec3(float x, float y, float z)>(), "vec3");
    m_chai->add(chaiscript::fun(&sm::vec3::operator=), "=");

    m_chai->add(chaiscript::user_type<sm::vec4>(), "vec4");
    m_chai->add(chaiscript::constructor<sm::vec4(float x, float y, float z, float w)>(), "vec4");
    m_chai->add(chaiscript::fun(&sm::vec4::operator=), "=");

    m_chai->add(chaiscript::user_type<sm::mat4>(), "mat4");
    m_chai->add(chaiscript::constructor<sm::mat4()>(), "mat4");
    m_chai->add(chaiscript::fun(&sm::mat4::operator=), "=");

    // Shader
    m_chai->add(chaiscript::user_type<node::Shader>(), "Shader");
    m_chai->add(
      chaiscript::fun<std::function<void(std::shared_ptr<node::Shader>& shader, const std::string& key, const sm::mat4& val)>>(
        [&](std::shared_ptr<node::Shader>& shader, const std::string& key, const sm::mat4& val) {
            //auto& rc = ur::Blackboard::Instance()->GetRenderContext();
            //shader->SetUniformValue(rc, key, ShaderVariant(val));
        }), "set_uniform");
    m_chai->add(
      chaiscript::fun<std::function<void(std::shared_ptr<node::Shader>& shader, const std::string& key, const sm::vec3& val)>>(
        [&](std::shared_ptr<node::Shader>& shader, const std::string& key, const sm::vec3& val) {
            //auto& rc = ur::Blackboard::Instance()->GetRenderContext();
            //shader->SetUniformValue(rc, key, ShaderVariant(val));
        }), "set_uniform");

    // for return vector
    m_chai->add(chaiscript::vector_conversion<std::vector<int>>());
    m_chai->add(chaiscript::vector_conversion<std::vector<float>>());
    m_chai->add(chaiscript::vector_conversion<std::vector<sm::vec2>>());
    m_chai->add(chaiscript::vector_conversion<std::vector<sm::vec3>>());
    m_chai->add(chaiscript::vector_conversion<std::vector<sm::vec4>>());

    // for std::vector []
    m_chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<float>>("Vec1Vector"));
    m_chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<sm::vec2>>("Vec2Vector"));
    m_chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<sm::vec3>>("Vec3Vector"));
    m_chai->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<sm::vec4>>("Vec4Vector"));

    // functions

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

    m_chai->add(
      chaiscript::fun<std::function<float(float deg)>>(
        [](float deg) {
            return deg * SM_DEG_TO_RAD;
        }), "radians");

    m_chai->add(
      chaiscript::fun<std::function<sm::vec3(const sm::vec3& v)>>(
        [](const sm::vec3& v) {
            return v.Normalized();
        }), "normalize");

    m_chai->add(
      chaiscript::fun<std::function<sm::vec3(const sm::vec3& v, const sm::vec3& offset)>>(
        [](const sm::vec3& v, const sm::vec3& offset) {
            return v + offset;
        }), "translate");

    m_chai->add(
      chaiscript::fun<std::function<sm::mat4(const sm::mat4& mt, const sm::vec3& offset)>>(
        [](const sm::mat4& mt, const sm::vec3& offset) {
            return sm::mat4::Translated(offset.x, offset.y, offset.z) * mt;
        }), "translate");

    m_chai->add(
      chaiscript::fun<std::function<sm::mat4(const sm::mat4& mt, float angle, const sm::vec3& axis)>>(
        [](const sm::mat4& mt, float angle, const sm::vec3& axis) {
            return sm::mat4::RotatedAxis(axis, angle) * mt;
        }), "rotate");

    m_chai->add(
      chaiscript::fun<std::function<sm::vec3(const sm::vec3& v, const sm::vec3& scale)>>(
        [](const sm::vec3& v, const sm::vec3& scale) {
            sm::vec3 ret;
            for (int i = 0; i < 3; ++i) {
                ret.xyz[i] = v.xyz[i] * scale.xyz[i];
            }
            return ret;
        }), "scale");

    m_chai->add(
      chaiscript::fun<std::function<sm::mat4(const sm::mat4& mt, const sm::vec3& scale)>>(
        [](const sm::mat4& mt, const sm::vec3& scale) {
            return sm::mat4::Scaled(scale.x, scale.y, scale.z) * mt;
        }), "scale");

    m_chai->add(
      chaiscript::fun<std::function<void()>>(
        [&]() {
            //auto& rc = ur::Blackboard::Instance()->GetRenderContext();
            //rc.RenderCube(ur::RenderContext::VertLayout::VL_POS_NORM_TEX);
        }), "render_cube");
}

}