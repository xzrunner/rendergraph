#pragma once

namespace rg
{

enum class VariableType
{
    Any,

    Port,

    Texture,
    RenderTarget,

    Model,

    // shader
    Vector1,
    Vector2,
    Vector3,
    Vector4,
    Vec4Array,
    Matrix2,
    Matrix3,
    Matrix4,
    Sampler2D,
    SamplerCube,
};

}