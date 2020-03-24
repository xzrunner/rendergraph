#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Cull : public Node
{
public:
    Cull()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    enum class Mode
    {
        Off = 0,
        Front,
        Back,
        FrontAndBack,
    };

    void SetMode(Mode mode) { m_mode = mode; }

private:
    Mode m_mode = Mode::Back;

    RTTR_ENABLE(Node)

}; // Cull

}
}