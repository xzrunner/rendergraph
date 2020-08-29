#pragma once

namespace rendergraph
{

enum class VariableType
{
    Any,

    Port,

    Texture,
    RenderTarget,
    Shader,

    Model,

    String,

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

    Vec1Array,
    Vec2Array,
    Vec3Array,
    Vec4Array,

    UserType,
};

}