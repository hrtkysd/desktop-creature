#pragma once

#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "PartId.h"

#include <cstdint>
#include <vector>

namespace Creature
{
    namespace Editor
    {
        class CAnimationEditor;
    }

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
            friend class Creature::Editor::CAnimationEditor;
        public:
            CAnimation();
            explicit CAnimation(
                float fDuration,
                const std::vector<CAnimationTrack>& vecAnimationTrack);
            virtual ~CAnimation();
        public:
            float GetDuration() const;
            const std::vector<CAnimationTrack>& GetAnimationTracks() const;
            const CAnimationTrack* FindAnimationTrack(const CAnimationTrackKey& key) const;

        private:
            bool SetDuration(float fDuration);
            bool AddOrUpdateKeyFrame(
                const CAnimationTrackKey& key,
                const FloatKeyFrame& keyFrame);
            bool AddAnimationTrack(CAnimationTrack&& animationTrack);
            bool RemoveAnimationTrackByKey(const CAnimationTrackKey& key);
            bool RemoveAnimationTrackByPartId(Creature::PartId id);
            CAnimationTrack* FindAnimationTrack(const CAnimationTrackKey& key);

        private:
            float m_fDuration = 0.0f;
            std::vector<CAnimationTrack> m_vecAnimationTrack;
        };
    } // namespace Animation
} // namespace Creature
