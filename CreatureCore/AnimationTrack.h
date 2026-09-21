#pragma once

#include "AnimationTrackKey.h"
#include "Interpolation.h"
#include "PartId.h"

#include <cstdint>
#include <vector>

namespace Creature
{
    namespace Animation
    {
        struct FloatKeyFrame
        {
            float fTime = 0.0f;
            float fValue = 0.0f;
            Interpolation eInterpolationToNext = Interpolation::Linear;
        };

        class CAnimationTrack
        {
        public:
            explicit CAnimationTrack(const CAnimationTrackKey& key);
        public:
            float Sample(float fTime) const;

            const CAnimationTrackKey& GetKey() const noexcept;

            const std::vector<FloatKeyFrame>& GetKeyFrames() const;
            void AddOrUpdateKeyFrame(const FloatKeyFrame& keyFrame);
        public:
            bool Matches(const CAnimationTrackKey& key) const noexcept;
        private:
            CAnimationTrackKey m_animationTrackKey;
            std::vector<FloatKeyFrame> m_vecKeyFrame;
        };
    } // namespace Animation
} // namespace Creature
