#pragma once

#include "rendergraph/Node.h"

#include <painting0/UniformNames.h>

namespace ur { class Device; class ShaderProgram; }

namespace rendergraph
{
namespace node
{

class Shader : public Node
{
public:
    enum OutputID
    {
        O_OUT = 0,

        O_MAX_NUM
    };

public:
    Shader()
    {
        m_exports.resize(O_MAX_NUM);
        m_exports[O_OUT] = {{ VariableType::Shader, "out" }};
    }

    void SetCodes(const std::string& vert, const std::string& frag);

    void Bind(RenderContext& rc);

    std::shared_ptr<ur::ShaderProgram> GetShader(const RenderContext& rc);

//    auto& GetUniformNames() const { return m_unif_names; }

    void SetUniformValue(const ur::Device* dev, const std::string& key,
        const ShaderVariant& val);

    static void GetCodeUniforms(const std::string& code,
        std::vector<Variable>& uniforms, std::set<std::string>& unique_names);

private:
    void Init(const ur::Device& dev);

    void SetUniformValue(const Variable& key, const ShaderVariant& val);
    void SetUniformValue(const std::string& name, const ShaderVariant& val);

private:
    std::shared_ptr<ur::ShaderProgram> m_prog = nullptr;

    std::vector<std::string> m_textures;

    //pt0::UniformNames m_unif_names;

    std::string m_model_mat_name;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Shader.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Shader

}
}