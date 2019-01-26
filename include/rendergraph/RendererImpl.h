#pragma once

#include "rendergraph/RenderBuffer.h"

#include <unirender/typedef.h>

#include <memory>

namespace pt0 { class Shader; }

namespace rg
{

template<typename T>
class RendererImpl
{
public:
    RendererImpl();
    ~RendererImpl();

protected:
    void FlushBuffer(ur::DRAW_MODE mode);

protected:
    RenderBuffer<T> m_buf;

    std::shared_ptr<pt0::Shader> m_shader = nullptr;

    uint32_t m_vbo = 0, m_ebo = 0;

    uint32_t m_tex_id = 0;

}; // RendererImpl

}

#include "rendergraph/RendererImpl.inl"