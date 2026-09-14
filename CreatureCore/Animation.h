#pragma once

#include <memory>

namespace Creature
{
    struct CreaturePose;

    namespace Animation
    {
        enum class AnimationState
        {
            Idle,
            Walk,
            Run,
            Sleep
        };

        class CAnimation
        {
        public:
            CAnimation();
        public:
            void Update(
                float deltaTime,
                AnimationState state,
                float velocityX);

            const CreaturePose& GetPose() const noexcept;

        private:
            float m_time = 0.0f;
            float m_blinkTimer = 0.0f;

            std::unique_ptr<Creature::CreaturePose> m_pose{};
        };
    } // namespace Animation
} // namespace Creature
