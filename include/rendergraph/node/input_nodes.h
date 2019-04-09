#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class CamProjMat : public Node
{
public:
    CamProjMat() { m_exports = {{{ VariableType::Matrix4, "out" }}}; }
    virtual void Execute(const RenderContext& rc) override {}
    RTTR_ENABLE(Node)
}; // CamProjMat

class CamViewMat : public Node
{
public:
    CamViewMat() { m_exports = {{{ VariableType::Matrix4, "out" }}}; }
    virtual void Execute(const RenderContext& rc) override {}
    RTTR_ENABLE(Node)
}; // CamViewMat

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