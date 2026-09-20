#pragma once

#include "AnimationId.h"
#include <string>

namespace Creature
{
    class CCreature;

    namespace Animation
    {
        class CAnimation;
        class CAnimationTrack;
    }
}

class CAnimationEditor
{
public:
    explicit CAnimationEditor(Creature::CCreature& creature);
public:
    void SetName(Creature::Animation::AnimationId id, const std::string& strName);
    void SetDuration(Creature::Animation::AnimationId id, float fDuration);
    void AddAnimationTrack(Creature::Animation::AnimationId id, Creature::Animation::CAnimationTrack&& animationTrack);
private:
    Creature::CCreature& m_creature;
};
