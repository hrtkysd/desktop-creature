#pragma once

#include "Transform2D.h"

#include <vector>
#include <string>

namespace Creature
{
    namespace Animation {
        enum class AnimationProperty
        {
            PositionX,
            PositionY,
            Rotation,
            ScaleX,
            ScaleY
        };

        struct FloatKeyFrame
        {
            float fTime = 0.0f;
            float fValue = 0.0f;
        };

        struct AnimationTrack
        {
            std::string strTargetPart;
            AnimationProperty eProperty;

            std::vector<FloatKeyFrame> vecKeyFrame;
        };
    } // namespace Animation
} // namespace Creature
