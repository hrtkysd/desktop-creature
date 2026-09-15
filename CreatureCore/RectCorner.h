#pragma once

#include "Vec2.h"

namespace Creature
{
    namespace Math
    {
        struct RectCorner
        {
            Vec2 topLeft{};
            Vec2 topRight{};
            Vec2 bottomRight{};
            Vec2 bottomLeft{};
        };
    } // namespace Math
} // namespace Creature
