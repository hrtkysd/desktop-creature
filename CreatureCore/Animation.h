#pragma once

#include "AnimationId.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "PartId.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Creature
{
    class CCreature;

    namespace Editor
    {
        class CAnimationEditor;
        class CCreatureEditor;
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
            friend class Creature::Editor::CCreatureEditor;
            friend class Creature::CCreature;
        private:
            explicit CAnimation(
                AnimationId id,
                const std::string& strName);
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
            float GetDuration() const;
            const std::vector<CAnimationTrack>& GetAnimationTracks() const;
            const CAnimationTrack* FindAnimationTrack(const CAnimationTrackKey& key) const;

        private:
            static CAnimation NewAnimation(
                AnimationId id,
                const std::string& strName);
            void SetAnimationId(AnimationId id);
            void SetName(const std::string& strName);
            bool SetDuration(float fDuration);
            bool AddOrUpdateKeyFrame(
                const CAnimationTrackKey& key,
                const FloatKeyFrame& keyFrame);
            bool AddAnimationTrack(CAnimationTrack&& animationTrack);
            bool RemoveAnimationTrackByKey(const CAnimationTrackKey& key);
            bool RemoveAnimationTrackByPartId(Creature::PartId id);
            CAnimationTrack* FindAnimationTrack(const CAnimationTrackKey& key);

        private:
            AnimationId m_animationId = INVALID_ANIMATION_ID;
            std::string m_strName;
            float m_fDuration = 0.0f;
            std::vector<CAnimationTrack> m_vecAnimationTrack;
        };
    } // namespace Animation
} // namespace Creature
