#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class Texture : public Node
{
public:
    Texture()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port,    "next" }},
            {{ VariableType::Texture, "tex" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    void SetFilepath(const std::string& filepath) { m_filepath = filepath; }
    void SetParams(int type, int width, int height, int format);

    unsigned int GetTexID() const { return m_texid; }

    // todo
    void SetTexID(unsigned int texid) { m_texid = texid; }

private:
    std::string m_filepath;

    int m_type = 0;

    int m_width = 0, m_height = 0;
    int m_format = 0;

    unsigned int m_texid = 0;

    RTTR_ENABLE(Node)

}; // Texture

}
}