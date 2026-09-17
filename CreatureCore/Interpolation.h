#pragma once

#include <cstdint>

namespace Creature
{
    namespace Animation
    {
        enum class Interpolation : std::uint8_t
        {
            Linear,
            SmoothStep,
            Step
        };
    }
}
