#pragma once

#include "AnimationId.h"
#include "PartId.h"

#include <string>
#include <vector>

namespace Creature
{
    class CCreature;

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
            explicit CAnimationEditor(CCreature& creature);

            bool SetName(Animation::AnimationId id, const std::string& name);
            bool SetDuration(Animation::AnimationId id, float duration);

            bool AddTrack(Animation::AnimationId id, Animation::CAnimationTrack&& track);
            bool RemoveTrack(Animation::AnimationId id, const Animation::CAnimationTrackKey& key);

            bool AddOrUpdateKeyFrame(
                Animation::AnimationId id,
                const Animation::CAnimationTrackKey& key,
                const Animation::FloatKeyFrame& frame);
            void RemovePart(Creature::PartId id);

            const std::vector<Animation::CAnimation>& GetAnimation() const;
        private:
            Creature::CCreature& m_creature;
        };
    }
}
