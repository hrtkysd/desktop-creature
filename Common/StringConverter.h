#pragma once

#include <string>
#include <string_view>

namespace StringConverter
{
    std::wstring Utf8ToWide(std::string_view str);
}
