#pragma once

#include "Interpolation.h"
#include "Part.h"
#include "Transform2D.h"

#include <vector>
#include <string>

namespace Creature
{
    namespace Animation
    {
        enum class AnimationProperty
        {
            None,
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
            Interpolation eInterpolationToNext = Interpolation::Linear;
        };

        class CAnimationTrack
        {
        public:
            float Sample(float fTime) const;

            PartId GetPartId() const;
            void SetPartId(PartId id);

            AnimationProperty GetAnimationProperty() const;
            void SetAnimationProperty(AnimationProperty eProperty);

            const std::vector<FloatKeyFrame>& GetKeyFrames() const;
            void AddOrUpdateKeyFrame(const FloatKeyFrame& keyFrame);
        private:
            PartId m_targetPartId = INVALID_PART_ID;
            AnimationProperty m_eProperty = AnimationProperty::None;

            std::vector<FloatKeyFrame> m_vecKeyFrame;
        };
    } // namespace Animation
} // namespace Creature
