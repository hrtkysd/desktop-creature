#pragma once

#include <cstdint>

enum class DocumentCommand : std::uint8_t
{
    SaveAs,
    LoadFrom
};
