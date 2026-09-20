#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationTrack.h"
#include "Creature.h"

using namespace Creature;
using namespace Creature::Animation;

CAnimationEditor::CAnimationEditor(CCreature& creature)
    : m_creature(creature)
{
}

void CAnimationEditor::SetName(AnimationId id, const std::string& strName)
{
    if (const auto animation = m_creature.FindAnimationById(id))
    {
        animation->SetName(strName);
    }
}

void CAnimationEditor::SetDuration(AnimationId id, float fDuration)
{
    if (const auto animation = m_creature.FindAnimationById(id))
    {
        animation->SetDuration(fDuration);
    }
}

void CAnimationEditor::AddAnimationTrack(AnimationId id, CAnimationTrack&& animationTrack)
{
    if (const auto animation = m_creature.FindAnimationById(id))
    {
        animation->AddAnimationTrack(std::move(animationTrack));
    }
}
