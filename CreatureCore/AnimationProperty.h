#pragma once

#include <cstdint>

namespace Creature
{
    namespace Animation
    {
        enum class AnimationProperty : std::uint8_t
        {
            None,
            PositionX,
            PositionY,
            Rotation,
            ScaleX,
            ScaleY
        };
    }
}
