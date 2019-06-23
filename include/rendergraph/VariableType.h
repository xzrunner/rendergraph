#pragma once

namespace rg
{

enum class VariableType
{
    Any,

    Port,

    Texture,
    RenderTarget,
    Shader,

    Model,

    // shader
    Int,
    Bool,
    Vector1,
    Vector2,
    Vector3,
    Vector4,
    Matrix2,
    Matrix3,
    Matrix4,
    Sampler2D,
    SamplerCube,

    Vec3Array,
    Vec4Array,

    UserType,
};

}