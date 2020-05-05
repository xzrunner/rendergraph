#pragma once

#include "rendergraph/Node.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/Evaluator.h"

namespace rendergraph
{
namespace node
{

class ProjectMat : public Node
{
public:
    ProjectMat() { m_exports = {{{ VariableType::Matrix4, "out" }}}; }
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override {
        var.mat4 = rc.cam_proj_mat;
    }
    RTTR_ENABLE(Node)
}; // ProjectMat

class ViewMat : public Node
{
public:
    ViewMat() { m_exports = {{{ VariableType::Matrix4, "out" }}}; }
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override {
        var.mat4 = rc.cam_view_mat;
    }
    RTTR_ENABLE(Node)
}; // ViewMat

class ModelMat : public Node
{
public:
    ModelMat() { m_exports = { {{ VariableType::Matrix4, "out" }} }; }
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override {
        flags |= Evaluator::FLAG_MODEL_MAT;
        var.mat4 = rc.model_mat;
    }
    RTTR_ENABLE(Node)
}; // ModelMat

class CameraPosition : public Node
{
public:
    CameraPosition() { m_exports = {{{ VariableType::Vector3, "out" }}}; }
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override {
        flags |= Evaluator::FLAG_CAMERA_POS;
        var.vec3 = rc.cam_position;
    }
    RTTR_ENABLE(Node)
}; // CameraPosition

class LightPosition : public Node
{
public:
    LightPosition() { m_exports = { {{ VariableType::Vector3, "out" }} }; }
    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override {
        var.vec3 = rc.light_position;
    }
    RTTR_ENABLE(Node)
}; // LightPosition

}
}