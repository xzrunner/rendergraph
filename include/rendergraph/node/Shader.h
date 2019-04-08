#pragma once

#include "rendergraph/Node.h"

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

    virtual void Execute(ur::RenderContext& rc) override;

    void SetCodes(const std::string& vert, const std::string& frag);

    void Bind();

private:
    std::string m_vert, m_frag;

    std::shared_ptr<ur::Shader> m_shader = nullptr;

    RTTR_ENABLE(Node)

}; // Shader

}
}