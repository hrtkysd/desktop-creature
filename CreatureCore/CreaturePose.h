#pragma once

#include "Transform2D.h"
#include "Vec2.h"

#include <vector>

namespace Creature
{
    /**
     * @brief Represents the complete visual pose of a creature at a specific point in time.
     */
    struct CreaturePose
    {
        Math::Transform2D rootTransform {}; // Root transform applied to the entire creature.
        Math::Vec2 eyeOffset {}; // Offset applied to the eyes for gaze movement.

        bool blink = false; // Indicates whether the creature is currently blinking.

        std::vector<Math::Transform2D> vecPartTransform; // Per-part transforms corresponding to the skeleton part indices.
    };
}
