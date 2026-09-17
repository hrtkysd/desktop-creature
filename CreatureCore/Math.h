#pragma once

namespace Creature
{
    namespace Math
    {
        inline float Lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }

        inline float SmoothStep(float t)
        {
            return t * t * (3.0f - 2.0f * t);
        }
    }
}
