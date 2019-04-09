#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class Viewport : public Node
{
public:
    Viewport()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    void SetParams(int x, int y, int w, int h);

private:
    int m_x = 0, m_y = 0;
    int m_w = 0, m_h = 0;

    RTTR_ENABLE(Node)

}; // Viewport

}
}
