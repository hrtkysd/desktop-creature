#pragma once

#include "CreaturePose.h"

namespace Creature
{
    class CSkeleton;

    namespace Animation
    {
        class CAnimation;

        enum class AnimationPlayerState
        {
            Stop,
            Play,
            Pause,
        };

        class CAnimationPlayer
        {
        public:
            void Play();
            void Stop();
            void Pause();

            void Update(float fDeltaTime);
            void SamplePose(const CAnimation& animation, const CSkeleton& skeleton);

            const CCreaturePose& GetPose() const noexcept;
        private:
            float m_fCurrentTime = 0.0f;
            float m_blinkTimer = 0.0f;

            AnimationPlayerState m_eAnimationPlayerState = AnimationPlayerState::Stop;
            CCreaturePose m_pose;
        };
    } // namespace Animation
} // namespace Creature
