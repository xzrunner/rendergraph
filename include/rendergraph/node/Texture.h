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
    void SetSize(int width, int height) { m_width = width; m_height = height; }

    unsigned int GetTexID() const { return m_texid; }

    // todo
    void SetTexID(unsigned int texid) { m_texid = texid; }

    enum class Type
    {
        Tex2D,
        TexCube,
    };

    enum class Format
    {
        RGBA8,
        RGBA4,
        RGB,
        RGB565,
        A8,
        Depth,
    };

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

    void SetType(Type type) { m_type = type; }
    void SetFormat(Format fmt) { m_format = fmt; }
    void SetWrapping(Wrapping wrap) { m_wrap = wrap; }
    void SetFiltering(Filtering filter) { m_filter = filter; }

private:
    std::string m_filepath;

    Type m_type = Type::Tex2D;

    int m_width = 0, m_height = 0;
    Format m_format = Format::RGBA8;

    Wrapping  m_wrap   = Wrapping::Repeat;
    Filtering m_filter = Filtering::Linear;

    unsigned int m_texid = 0;

    RTTR_ENABLE(Node)

}; // Texture

}
}