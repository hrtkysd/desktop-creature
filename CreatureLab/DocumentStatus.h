#pragma once

#include <cstdint>

enum class DocumentStatus : std::uint8_t
{
    Empty,
    New,
    File,
};
