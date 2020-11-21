#pragma once

#include "rendergraph/Node.h"

#include <unirender/typedef.h>
#include <shadergraph/VarType.h>

namespace ur { class ShaderProgram; class Device; }
namespace shadergraph { struct Variant; }

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

    void Init(const ur::Device& dev, const std::string& vs, const std::string& fs,
        const std::vector<std::pair<std::string, ur::TexturePtr>>& textures,
        const std::vector<shadergraph::Variant>& input_vars);

    auto GetShader() const { return m_prog; }

    void SetInputVar(const std::string& name, const ur::TexturePtr& tex);

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