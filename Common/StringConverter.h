#pragma once

#include <string>
#include <Stringapiset.h>

namespace StringConverter
{
    std::wstring Utf8ToWide(std::string_view str);
}
