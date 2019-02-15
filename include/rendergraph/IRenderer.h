#pragma once

#include <memory>

namespace pt0 { class Shader; }

namespace rg
{

class IRenderer
{
public:
	virtual ~IRenderer() {}
	virtual void Flush() = 0;

    auto& GetShader() const { return m_shader; }

protected:
    std::shared_ptr<pt0::Shader> m_shader = nullptr;

}; // IRenderer

}