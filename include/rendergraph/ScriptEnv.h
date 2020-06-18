#pragma once

#include <memory>

namespace chaiscript { class ChaiScript; }

namespace rendergraph
{

class RenderContext;

class ScriptEnv
{
public:
    ScriptEnv();

    auto GetChai() const { return m_chai; }

    void SetRenderContext(const std::shared_ptr<RenderContext>& rc) { m_rc = rc; }

private:
    std::shared_ptr<chaiscript::ChaiScript> m_chai = nullptr;

    std::shared_ptr<RenderContext> m_rc = nullptr;

}; // ScriptEnv

}