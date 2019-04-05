#pragma once

#include "rendergraph/VariableType.h"

#include <string>

namespace rg
{

struct Variable
{
    VariableType type;
    std::string  name;

}; // Variable

}