#pragma once

#include <string>
#include <Stringapiset.h>

namespace StringConverter
{
    std::wstring Utf8ToWide(std::string_view str)
    {
        if (str.empty()) return {};

        const auto size = MultiByteToWideChar(
            CP_UTF8,
            0,
            str.data(),
            static_cast<int>(str.size()),
            nullptr,
            0);

        std::wstring result(size, L'\0');

        MultiByteToWideChar(
            CP_UTF8,
            0,
            str.data(),
            static_cast<int>(str.size()),
            result.data(),
            size);

        return result;
    }
}
