#pragma once

#include <painting0/Color.h>

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class Clear : public Node
{
public:
    virtual void Execute(ur::RenderContext& rc) override;

    void SetClearType(uint32_t type) { m_clear_type = type; }

    void SetColor(const pt0::Color& col) { m_col = col; }

public:
    static const uint32_t CLEAR_COLOR   = 0x1;
    static const uint32_t CLEAR_DEPTH   = 0x2;
    static const uint32_t CLEAR_STENCIL = 0x4;

private:
    uint32_t m_clear_type = 0;

    pt0::Color m_col = pt0::Color(0, 0, 0, 0);

    RTTR_ENABLE(Node)

}; // Clear

}
}