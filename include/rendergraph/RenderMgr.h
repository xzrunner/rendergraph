#pragma once

#include <cu/cu_macro.h>

#include <memory>
#include <array>

namespace rg
{

class IRenderer;

enum class RenderType
{
	NIL = 0,

	SPRITE,
	EXTERN,

	MAX_COUNT
};

class RenderMgr
{
public:
	std::shared_ptr<IRenderer> SetRenderer(RenderType type);

	void Flush();

private:
	std::array<std::shared_ptr<IRenderer>, static_cast<int>(RenderType::MAX_COUNT)> m_renderers;

	RenderType m_curr_render = RenderType::NIL;

	CU_SINGLETON_DECLARATION(RenderMgr);

}; // RenderMgr

}