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

    enum class Wrapping
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };

    enum class Filtering
    {
        Nearest,
        Linear,
    };

    void SetWrapping(Wrapping wrap) { m_wrap = wrap; }
    void SetFiltering(Filtering filter) { m_filter = filter; }

private:
    std::string m_filepath;

    int m_type = 0;

    int m_width = 0, m_height = 0;
    int m_format = 0;

    unsigned int m_texid = 0;

    Wrapping  m_wrap   = Wrapping::Repeat;
    Filtering m_filter = Filtering::Linear;

    RTTR_ENABLE(Node)

}; // Texture

}
}