#pragma once

#include "AnimationId.h"
#include "AnimationTrack.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Creature
{
    class CCreature;

    namespace Animation
    {
        enum class AnimationState : std::uint8_t
        {
            Idle,
            Walk,
            Run,
            Sleep
        };

        class CAnimation
        {
            friend class Creature::CCreature;
        private:
            explicit CAnimation(
                AnimationId id,
                const std::string& strName);
        private:
            static CAnimation NewAnimation(AnimationId id, const std::string& strName);
        public:
            CAnimation() = delete;
            explicit CAnimation(
                AnimationId id,
                const std::string& strName,
                float fDuration,
                const std::vector<CAnimationTrack>& vecAnimationTrack);
            virtual ~CAnimation();
        public:

            AnimationId GetAnimationId() const;

            const std::string& GetName() const;
            void SetName(const std::string& strName);

            float GetDuration() const;
            void SetDuration(float fDuration);

            const std::vector<CAnimationTrack>& GetAnimationTrack() const;
            void AddAnimationTrack(CAnimationTrack&& animationTrack);
        private:
            AnimationId m_animationId = INVALID_ANIMATION_ID;
            std::string m_strName;
            float m_fDuration = 0.0f;
            std::vector<CAnimationTrack> m_vecAnimationTrack;
        };
    } // namespace Animation
} // namespace Creature
