#pragma once

#include "rendergraph/IRenderer.h"

#include <SM_Vector.h>

#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>

namespace pt0 { class Shader; }

namespace rg
{

class Tex3dRenderer : public IRenderer, boost::noncopyable
{
public:
	Tex3dRenderer();
	~Tex3dRenderer();

	virtual void Flush() override;

	void DrawCube(const float* positions, const float* texcoords, int texid, uint32_t color);

public:
	struct Vertex
	{
		sm::vec3 pos;
		sm::vec3 uv;
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

	void PrepareRenderState();
	void RestoreRenderState();

private:
	Buffer m_buf;

	std::shared_ptr<pt0::Shader> m_shader = nullptr;

	uint32_t m_vbo = 0, m_ebo = 0;

	uint32_t m_tex_id = 0;

}; // Tex3dRenderer

}