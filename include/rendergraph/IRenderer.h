#pragma once

namespace rg
{

class IRenderer
{
public:
	virtual ~IRenderer() {}
	virtual void Flush() = 0;

}; // IRenderer

}