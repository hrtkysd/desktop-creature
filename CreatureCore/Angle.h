#pragma once

namespace Creature
{
    namespace Math
    {
        namespace Angle
        {
            constexpr float PI = 3.14159265358979323846f;
            constexpr float TWO_PI = PI * 2.0f;

            inline float Normalize(float angle)
            {
                while (angle > PI) angle -= TWO_PI;
                while (angle < -PI) angle += TWO_PI;
                return angle;
            }
        }
    }
}
