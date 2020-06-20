#pragma once

#include <unirender/typedef.h>

namespace ur { class TextureSampler; }

namespace rendergraph
{

struct TextureVal
{
	ur::TexturePtr                      texture = nullptr;
	std::shared_ptr<ur::TextureSampler> sampler = nullptr;
};

}