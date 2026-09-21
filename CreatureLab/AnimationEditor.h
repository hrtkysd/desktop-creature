#pragma once

#include "AnimationId.h"
#include "PartId.h"

#include <cstdint>
#include <string>

namespace Creature
{
    class CCreature;

    namespace Animation
    {
        class CAnimationTrack;
        class CAnimationTrackKey;
    }
}

class CAnimationEditor
{
public:
    explicit CAnimationEditor(Creature::CCreature& creature);
public:
    bool SetName(
        Creature::Animation::AnimationId id,
        const std::string& strName);
    bool SetDuration(
        Creature::Animation::AnimationId id,
        float fDuration);
    bool AddAnimationTrack(
        Creature::Animation::AnimationId id,
        Creature::Animation::CAnimationTrack&& animationTrack);
    bool RemoveAnimationTrack(
        Creature::Animation::AnimationId id,
        const Creature::Animation::CAnimationTrackKey& trackKey);
private:
    Creature::CCreature& m_creature;
};
