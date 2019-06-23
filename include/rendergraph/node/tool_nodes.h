//#pragma once
//
//#include "rendergraph/Node.h"
//#include "rendergraph/Evaluator.h"
//
//namespace rg
//{
//namespace node
//{
//
//class For : public Node
//{
//public:
//    For()
//    {
//        m_imports = {
//            {{ VariableType::Port, "prev" }},
//            {{ VariableType::Any,  "in" }},
//        };
//        m_exports = {
//            {{ VariableType::Port,    "next" }},
//            {{ VariableType::Any,     "out" }},
//            {{ VariableType::Vector1, "index" }}
//        };
//    }
//
//    virtual void Execute(const RenderContext& rc)
//    {
//        if (m_imports[ID_IN].conns.empty()) {
//            return;
//        }
//
//        auto node = m_imports[ID_IN].conns[0].node.lock();
//        if (node) {
//            for (int i = m_index_begin; i < m_index_end; i += m_index_step) {
//                node->Execute(rc);
//            }
//        }
//    }
//
//    virtual void Eval(const RenderContext& rc, size_t port_idx,
//                      ShaderVariant& var, uint32_t& flags) const override
//    {
//        switch (port_idx)
//        {
//        case ID_OUT:
//            for (int i = m_index_begin; i < m_index_end; i += m_index_step)
//            {
//                m_index_curr = i;
//                Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
//            }
//            break;
//        case ID_INDEX:
//            var.type = VariableType::Int;
//            var.i    = m_index_curr;
//            break;
//        }
//    }
//
//    void SetProps(int i_begin, int i_end, int i_step) {
//        m_index_begin = i_begin;
//        m_index_end   = i_end;
//        m_index_step  = i_step;
//    }
//
//    enum InputID
//    {
//        ID_IN= 1,
//    };
//
//    enum OutputID
//    {
//        ID_OUT = 1,
//        ID_INDEX,
//    };
//
//private:
//    int m_index_begin = 0;
//    int m_index_end   = 0;
//
//    int m_index_step = 0;
//
//    mutable int m_index_curr = 0;
//
//    RTTR_ENABLE(Node)
//
//}; // For
//
//}
//}