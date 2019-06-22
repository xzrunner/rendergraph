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
    Shader()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    void SetCodes(const std::string& vert, const std::string& frag);

    void Bind(const RenderContext& rc);

    std::shared_ptr<ur::Shader> GetShader(const RenderContext& rc);

    auto& GetUniformNames() const { return m_unif_names; }

    static void GetCodeUniforms(const std::string& code,
        std::vector<Variable>& uniforms, std::set<std::string>& unique_names);

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