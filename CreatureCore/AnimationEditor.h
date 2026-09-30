#pragma once

#include "AnimationId.h"
#include "PartId.h"

#include <string>
#include <vector>

namespace Creature
{
    namespace Animation
    {
        class CAnimation;
        class CAnimationTrack;
        class CAnimationTrackKey;

        struct FloatKeyFrame;
    }
    namespace Editor
    {
        class CAnimationEditor
        {
        public:
            explicit CAnimationEditor(
                std::vector<Animation::CAnimation>& vecAnimation);

            bool SetName(Animation::AnimationId id, const std::string& name);
            bool SetDuration(Animation::AnimationId id, float duration);

            bool AddTrack(Animation::AnimationId id, Animation::CAnimationTrack&& track);
            bool RemoveTrack(Animation::AnimationId id, const Animation::CAnimationTrackKey& key);

            bool AddOrUpdateKeyFrame(
                Animation::AnimationId id,
                const Animation::CAnimationTrackKey& key,
                const Animation::FloatKeyFrame& frame);
            void RemovePart(Creature::PartId id);

            const std::vector<Animation::CAnimation>& GetAnimations() const;
        private:
            Animation::CAnimation* FindAnimationById(Animation::AnimationId id);
        private:
            std::vector<Creature::Animation::CAnimation>& m_vecAnimation;
        };
    }
}
