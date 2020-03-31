#pragma once

#include "rendergraph/Node.h"

#include <painting0/UniformNames.h>

namespace ur { class Shader; class RenderContext; }

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

    void Bind(const RenderContext& rc);

    std::shared_ptr<ur::Shader> GetShader(const ur::RenderContext& ur_rc);

    auto& GetUniformNames() const { return m_unif_names; }

    void SetUniformValue(ur::RenderContext& ur_rc, const std::string& key,
        const ShaderVariant& val);

    static void GetCodeUniforms(const std::string& code,
        std::vector<Variable>& uniforms, std::set<std::string>& unique_names);

private:
    void Init(const ur::RenderContext& ur_rc);

    void SetUniformValue(const Variable& key, const ShaderVariant& val,
        std::vector<uint32_t>& texture_ids);

private:
    std::shared_ptr<ur::Shader> m_shader = nullptr;

    std::vector<std::string> m_textures;

    pt0::UniformNames m_unif_names;

    std::string m_model_mat_name;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Shader.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Shader

}
}