#pragma once

#include "rendergraph/IRenderer.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>

namespace pt0 { class Shader; }
namespace tess { class Painter; class Palette; }
namespace gs { class Shape; }

namespace rg
{

class SpriteRenderer : public IRenderer, boost::noncopyable
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	virtual void Flush() override;

	void DrawQuad(const float* positions, const float* texcoords, int texid, uint32_t color);
	void DrawPainter(const tess::Painter& pt, const sm::mat4& mat = sm::mat4());

	auto GetShader() { return m_shader; }

	auto& GetPalette() const { return *m_palette; }

public:
	struct Vertex
	{
		sm::vec2 pos;
		sm::vec2 uv;
		uint32_t col = 0;
	};

	struct Buffer
	{
		void Reserve(size_t idx_count, size_t vtx_count);

		void Clear();

		std::vector<Vertex>         vertices;
		std::vector<unsigned short> indices;

		unsigned short  curr_index = 0;
		Vertex*         vert_ptr = nullptr;
		unsigned short* index_ptr = nullptr;
	};

private:
	void InitShader();

private:
	Buffer m_buf;

	std::shared_ptr<pt0::Shader> m_shader = nullptr;

	std::unique_ptr<tess::Palette> m_palette = nullptr;

	uint32_t m_vbo = 0, m_ebo = 0;

	uint32_t m_tex_id = 0;

}; // SpriteRenderer

}