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
    virtual void Execute(ur::RenderContext& rc) override;

    void SetCodes(const std::string& vert, const std::string& frag);

    auto GetShader() { return m_shader; }

private:
    std::string m_vert, m_frag;

    std::shared_ptr<ur::Shader> m_shader = nullptr;

    RTTR_ENABLE(Node)

}; // Shader

}
}