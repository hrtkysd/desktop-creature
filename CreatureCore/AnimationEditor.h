#pragma once

#include "PartId.h"

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
                Animation::CAnimation& animation);

            bool SetDuration(float duration);

            bool AddTrack(Animation::CAnimationTrack&& track);
            bool RemoveTrack(const Animation::CAnimationTrackKey& key);

            bool AddOrUpdateKeyFrame(
                const Animation::CAnimationTrackKey& key,
                const Animation::FloatKeyFrame& frame);
            void RemovePart(Creature::PartId id);

            const Animation::CAnimation& GetAnimation() const;
        private:
            Creature::Animation::CAnimation& m_animation;
        };
    }
}
