#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class RenderTarget : public Node
{
public:
    RenderTarget()
    {
        m_imports = {
            {{ VariableType::Port,    "prev" }},
            {{ VariableType::Texture, "tex" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }
    virtual ~RenderTarget();

    virtual void Execute(const RenderContext& rc) override;

    unsigned int GetID() const { return m_id; }

    bool IsBinded() const { return m_binded; }
    void SetBinded(bool binded) { m_binded = binded; }

    void SetLastViewport(int x, int y, int w, int h);
    void GetLastViewport(int& x, int& y, int& w, int& h) const;

private:
    unsigned int m_id = 0;

    bool m_binded = false;

    // last viewport
    int m_vp_x = 0, m_vp_y = 0, m_vp_w = 0, m_vp_h = 0;

    RTTR_ENABLE(Node)

}; // RenderTarget

}
}