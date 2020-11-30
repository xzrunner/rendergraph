#pragma once

#include <painting0/Color.h>

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Barrier : public Node
{
public:
    enum class Type
    {
        VertexAttribArray, 
        ElementArray, 
        Uniform, 
        TextureFetch, 
        ShaderImageAccess, 
        Command, 
        PixelBuffer, 
        TextureUpdate, 
        BufferUpdate, 
        Framebuffer, 
        TransformFeedback, 
        AtomicCounter, 
        ShaderStorage
    };

public:
    Barrier()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }},
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Barrier.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Barrier

}
}