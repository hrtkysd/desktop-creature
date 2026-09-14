#pragma once

#include "Transform2D.h"

namespace Creature
{
    namespace Animation
    {
        /**
         * @brief Represents per-part pose of creature at a specific point in time. 
         */
        struct PartPose
        {
            Math::Transform2D transform{};
        };
    } // namespace Animation
} // namespace Creature
