#pragma once

#include "rendergraph/Node.h"

#include <painting0/UniformNames.h>

namespace ur { class ShaderProgram; }

namespace rendergraph
{
namespace node
{

class Shader : public Node
{
public:
    enum InputID
    {
        I_VS = 0,
        I_FS,

        I_MAX_NUM
    };

    enum OutputID
    {
        O_OUT = 0,

        O_MAX_NUM
    };

    enum class Language
    {
        GLSL,
        HLSL
    };

public:
    Shader()
    {
        m_imports.resize(I_MAX_NUM);
        m_imports[I_VS] = { { VariableType::String, "vs" } };
        m_imports[I_FS] = { { VariableType::String, "fs" } };

        m_exports.resize(O_MAX_NUM);
        m_exports[O_OUT] = {{ VariableType::Shader, "out" }};
    }

    void SetCodes(const std::string& vert, const std::string& frag,
        std::ostream& out);

    void Bind(RenderContext& rc);

    std::shared_ptr<ur::ShaderProgram> GetShader(const RenderContext& rc);

//    auto& GetUniformNames() const { return m_unif_names; }

    void SetUniformValue(const RenderContext& rc, const std::string& key,
        const ShaderVariant& val);

private:
    void Init(const RenderContext& rc);

    void SetUniformValue(const Variable& key, const ShaderVariant& val);
    void SetUniformValue(const std::string& base_name, const std::string& name,
        const ShaderVariant& val);

private:
    std::shared_ptr<ur::ShaderProgram> m_prog = nullptr;

    //pt0::UniformNames m_unif_names;

    std::string m_model_mat_name;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Shader.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Shader

}
}