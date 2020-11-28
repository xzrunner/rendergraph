#pragma once

#include "rendergraph/VariableType.h"

#include <string>

namespace rendergraph
{

struct Variable
{
    VariableType type;
    std::string  name;

    int count = 1; // for array
    std::string user_type;

    std::string base_name;

    //Variable() {}
    //Variable(VariableType type, const std::string& name)
    //    : type(type), name(name) {}

    std::string GetDisplayName() const
    {
        if (base_name.empty() && count == 1) {
            return name;
        }

        std::string ret = name;
        if (!base_name.empty()) {
            ret = base_name + "." + name;
        }
        if (count != 1) {
            ret += "[" + std::to_string(count) + "]";
        }
        return ret;
    }

}; // Variable

}