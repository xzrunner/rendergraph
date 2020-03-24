#include "rendergraph/Utility.h"

#include <cpputil/StringHelper.h>

namespace rendergraph
{

std::string Utility::FormatCode(const std::string& code)
{
    auto ret = code;

    cpputil::StringHelper::ReplaceAll(ret, "\\n", "\n");
    cpputil::StringHelper::ReplaceAll(ret, "\\t", "\t");

    return ret;
}

}