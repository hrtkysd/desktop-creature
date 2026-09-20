#pragma once

#include <cstdint>

namespace Creature
{
    namespace Animation
    {
        using AnimationId = std::uint32_t;

        constexpr AnimationId INVALID_ANIMATION_ID = 0;
        constexpr AnimationId MIN_ANIMATION_ID = 1;
    }
}
