#pragma once

#include "AnimationTrack.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Creature
{
    class CCreature;
    namespace Animation
    {
        enum class AnimationState
        {
            Idle,
            Walk,
            Run,
            Sleep
        };

        using AnimationId = std::uint32_t;
        constexpr AnimationId INVALID_ANIMATION_ID = 0;
        constexpr AnimationId MIN_ANIMATION_ID = 1;

        class CAnimation
        {
            friend class Creature::CCreature;
        private:
            explicit CAnimation(
                AnimationId id,
                const std::string& strName);
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
