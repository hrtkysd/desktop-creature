#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Creature.h"

using namespace Creature;
using namespace Creature::Animation;

CAnimationEditor::CAnimationEditor(CCreature& creature)
    : m_creature(creature)
{
}

bool CAnimationEditor::SetName(
    AnimationId id,
    const std::string& strName)
{
    if (const auto animation = m_creature.FindAnimationById(id))
    {
        animation->SetName(strName);
        return true;
    }
    return false;
}

bool CAnimationEditor::SetDuration(
    AnimationId id,
    float fDuration)
{
    if (const auto animation = m_creature.FindAnimationById(id))
    {
        animation->SetDuration(fDuration);
        return true;
    }
    return false;
}

bool CAnimationEditor::AddAnimationTrack(
    AnimationId id,
    CAnimationTrack&& animationTrack)
{
    if (const auto animation = m_creature.FindAnimationById(id))
    {
        return animation->AddAnimationTrack(std::move(animationTrack));
    }
    return false;
}

bool CAnimationEditor::RemoveAnimationTrack(
    AnimationId id,
    const CAnimationTrackKey& trackKey)
{
    if (const auto animation = m_creature.FindAnimationById(id))
    {
        return animation->RemoveAnimationTrack(trackKey);
    }
    return false;
}
