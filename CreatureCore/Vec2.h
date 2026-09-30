#pragma once

namespace Creature
{
    namespace Math
    {
        struct Vec2
        {
            float x = 0.0f;
            float y = 0.0f;
            inline bool operator==(const Vec2& rhs) const
            {
                return x == rhs.x && y == rhs.y;
            }
        };
    }
}
