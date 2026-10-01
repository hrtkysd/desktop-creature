#include "StringConverter.h"

#include <Windows.h>

std::wstring StringConverter::Utf8ToWide(std::string_view str)
{
    if (str.empty()) return {};

    auto size = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        static_cast<int>(str.size()),
        nullptr,
        0);
    if (size == 0) return {};

    std::wstring result(size, L'\0');

    size = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        static_cast<int>(str.size()),
        result.data(),
        size);
    if (size == 0) return {};

    return result;
}
