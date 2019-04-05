#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class Texture : public Node
{
public:
    virtual void Execute(ur::RenderContext& rc) override;

    void SetFilepath(const std::string& filepath) { m_filepath = filepath; }
    void SetParams(int type, int width, int height, int format);

    unsigned int GetTexID() const { return m_texid; }

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