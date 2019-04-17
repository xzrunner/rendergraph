#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class ProjectMat : public Node
{
public:
    ProjectMat() { m_exports = {{{ VariableType::Matrix4, "out" }}}; }
    virtual void Execute(const RenderContext& rc) override {}
    RTTR_ENABLE(Node)
}; // ProjectMat

class ViewMat : public Node
{
public:
    ViewMat() { m_exports = {{{ VariableType::Matrix4, "out" }}}; }
    virtual void Execute(const RenderContext& rc) override {}
    RTTR_ENABLE(Node)
}; // ViewMat

class ModelMat : public Node
{
public:
    ModelMat() { m_exports = { {{ VariableType::Matrix4, "out" }} }; }
    virtual void Execute(const RenderContext& rc) override {}
    RTTR_ENABLE(Node)
}; // ModelMat

class CameraPosition : public Node
{
public:
    CameraPosition() { m_exports = {{{ VariableType::Vector3, "out" }}}; }
    virtual void Execute(const RenderContext& rc) override {}
    RTTR_ENABLE(Node)
}; // CameraPosition

class LightPosition : public Node
{
public:
    LightPosition() { m_exports = { {{ VariableType::Vector3, "out" }} }; }
    virtual void Execute(const RenderContext& rc) override {}
    RTTR_ENABLE(Node)
}; // LightPosition

}
}