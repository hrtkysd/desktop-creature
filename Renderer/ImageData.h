#pragma once

#include <cstdint>
#include <vector>

struct ImageData
{
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::vector<std::uint8_t> pixels;
};
