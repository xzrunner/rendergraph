#pragma once

#include "rendergraph/Node.h"

#include <unirender/typedef.h>

namespace ur { class ShaderProgram; class Device; }

namespace rendergraph
{
namespace node
{

class ShaderGraph : public Node
{
public:
    enum class VertexShader
    {
        Image,
        Model
    };

public:
    ShaderGraph()
    {
        m_exports = {
            {{ VariableType::Shader, "out" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    void Init(const ur::Device& dev, const std::string& fs,
        const std::vector<std::pair<size_t, ur::TexturePtr>>& textures);

private:
    std::shared_ptr<ur::ShaderProgram> m_prog = nullptr;

    std::vector<std::pair<size_t, ur::TexturePtr>> m_textures;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/ShaderGraph.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ShaderGraph

}
}