#pragma once

#include "rendergraph/IRenderer.h"
#include "rendergraph/RendererImpl.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <boost/noncopyable.hpp>

namespace pt2 { class WindowContext; }
namespace tess { class Painter; class Palette; }

namespace rg
{

struct SpriteVertex
{
	sm::vec2 pos;
	sm::vec2 uv;
	uint32_t col = 0;
};

class SpriteRenderer : public IRenderer, private RendererImpl<SpriteVertex>, private boost::noncopyable
{
public:
	SpriteRenderer();

	virtual void Flush() override;

	void DrawQuad(const float* positions, const float* texcoords, int texid, uint32_t color);
	void DrawPainter(const tess::Painter& pt, const sm::mat4& mat = sm::mat4());

	auto GetShader() { return m_shader; }

	auto& GetPalette() const { return *m_palette; }

    void BindWindowContext(pt2::WindowContext& wc);

private:
	void InitShader();

private:
	std::unique_ptr<tess::Palette> m_palette = nullptr;

}; // SpriteRenderer

}