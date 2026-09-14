#pragma once

#include "Vec2.h"

namespace Creature
{
    namespace Math
    {
        struct Transform2D
        {
            Vec2 position{};
            Vec2 scale{ 1.0f, 1.0f };
            float rotation = 0.0f;
        };
    }
}
