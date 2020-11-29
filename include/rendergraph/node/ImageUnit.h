#pragma once

#include <unirender/AccessType.h>
#include <unirender/ImageFormat.h>

#include <string>

namespace rendergraph
{
namespace node
{

struct ImageUnit
{
    std::string name;

    int unit = 0;

    ur::ImageFormat fmt;

    ur::AccessType access;
};

}
}