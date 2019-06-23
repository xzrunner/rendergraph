#pragma once

#include "rendergraph/Node.h"

#include <painting0/UniformNames.h>

namespace ur { class Shader; }

namespace rg
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
        m_exports = {
            {{ VariableType::Shader, "out" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    void SetCodes(const std::string& vert, const std::string& frag);

    void Bind(const RenderContext& rc);

    std::shared_ptr<ur::Shader> GetShader(const RenderContext& rc);

    auto& GetUniformNames() const { return m_unif_names; }

    void SetUniformValue(const std::string& key, const ShaderVariant& val);

    static void GetCodeUniforms(const std::string& code,
        std::vector<Variable>& uniforms, std::set<std::string>& unique_names);

private:
    void SetUniformValue(const Variable& key, const ShaderVariant& val,
        std::vector<uint32_t>& texture_ids);

private:
    std::string m_vert, m_frag;

    std::shared_ptr<ur::Shader> m_shader = nullptr;

    std::vector<std::string> m_textures;

    pt0::UniformNames m_unif_names;

    std::string m_model_mat_name;

    RTTR_ENABLE(Node)

}; // Shader

}
}