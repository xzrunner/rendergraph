#pragma once

#include <painting0/Color.h>

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class AttachRT : public Node
{
public:
	enum class AttachmentType
	{
		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,
		Color8,
		Color9,
		Color10,
		Color11,
		Color12,
		Color13,
		Color14,
		Color15,
		Depth,
		Stencil,
	};

	enum class TextureTarget
	{
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCubeMap,
		TextureCubeMap0,
		TextureCubeMap1,
		TextureCubeMap2,
		TextureCubeMap3,
		TextureCubeMap4,
		TextureCubeMap5,
	};

	enum InputID
	{
		I_RT = 1,
		I_TEX
	};

public:
    AttachRT()
    {
        m_imports = {
            {{ VariableType::Port,         "prev" }},
			{{ VariableType::RenderTarget, "rt" }},
			{{ VariableType::Texture,      "tex" }},
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/AttachRT.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // AttachRT

}
}