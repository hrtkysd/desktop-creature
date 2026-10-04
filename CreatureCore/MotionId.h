#pragma once

#include <cstdint>

namespace Creature
{
    using MotionId = std::uint32_t;

    constexpr MotionId INVALID_MOTION_ID = 0;
    constexpr MotionId MIN_MOTION_ID = 1;
}
