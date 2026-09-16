#pragma once

#include <string>
#include <vector>

namespace Creature
{
    namespace Animation
    {
        enum class AnimationState
        {
            Idle,
            Walk,
            Run,
            Sleep
        };

        class CAnimationTrack;
        class CAnimation
        {
        public:
            CAnimation();
            CAnimation(const std::string& strName, float fDuration, const std::vector<CAnimationTrack>& vecAnimationTrack);
            virtual ~CAnimation();
        public:
            const std::string& GetName() const;

            float GetDuration() const;
            void SetDuration(float fDuration);

            const std::vector<CAnimationTrack>& GetAnimationTrack() const;
            void AddAnimationTrack(CAnimationTrack&& animationTrack);
        private:
            std::string m_strName;
            float m_fDuration = 0.0f;
            std::vector<CAnimationTrack> m_vecAnimationTrack;
        };
    } // namespace Animation
} // namespace Creature
