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

    void SetCodes(const std::string& vert, const std::string& tess_ctrl, 
        const std::string& tess_eval, const std::string& frag, std::ostream& out);

    void Bind(RenderContext& rc);

    std::shared_ptr<ur::ShaderProgram> GetShader(const RenderContext& rc);

    void SetUniformValue(const RenderContext& rc, const std::string& key,
        const ShaderVariant& val);

    auto& GetErrorLog() const { return m_err_log; }

private:
    void Init(const RenderContext& rc);

private:
    std::shared_ptr<ur::ShaderProgram> m_prog = nullptr;

    std::string m_err_log;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Shader.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Shader

}
}