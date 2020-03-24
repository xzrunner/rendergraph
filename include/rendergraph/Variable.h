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

    std::string GetDisplayName() 
    {
        if (user_type.empty() && count == 1) {
            return name;
        }

        std::string ret = name;
        if (!user_type.empty()) {
            ret = user_type + "." + name;
        }
        if (count != 1) {
            ret += "[" + std::to_string(count) + "]";
        }
        return ret;
    }
    
}; // Variable

}