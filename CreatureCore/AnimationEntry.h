#pragma once

#include "Animation.h"

namespace Creature
{
    namespace Animation
    {
        struct AnimationEntry
        {
            AnimationId id = INVALID_ANIMATION_ID;
            CAnimation animation;
        };
    }
}
