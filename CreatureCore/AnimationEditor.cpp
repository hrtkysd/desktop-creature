#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Creature.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

CAnimationEditor::CAnimationEditor(CCreature& creature)
    : m_creature(creature)
{
}

bool CAnimationEditor::SetName(
    AnimationId id,
    const std::string& strName)
{
    if (auto animation = m_creature.FindAnimationById(id))
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
    if (auto animation = m_creature.FindAnimationById(id))
    {
        animation->SetDuration(fDuration);
        return true;
    }
    return false;
}

bool CAnimationEditor::AddTrack(
    AnimationId id,
    CAnimationTrack&& animationTrack)
{
    if (auto animation = m_creature.FindAnimationById(id))
    {
        return animation->AddAnimationTrack(std::move(animationTrack));
    }
    return false;
}

bool CAnimationEditor::RemoveTrack(
    AnimationId id,
    const CAnimationTrackKey& trackKey)
{
    if (auto animation = m_creature.FindAnimationById(id))
    {
        return animation->RemoveAnimationTrackByKey(trackKey);
    }
    return false;
}

bool CAnimationEditor::AddOrUpdateKeyFrame(
    AnimationId id,
    const CAnimationTrackKey& trackKey,
    const FloatKeyFrame& keyFrame)
{
    if (auto animation = m_creature.FindAnimationById(id))
    {
        return animation->AddOrUpdateKeyFrame(trackKey, keyFrame);
    }
    return false;
}

void CAnimationEditor::RemovePart(PartId id)
{
    for (auto& animation : m_creature.GetAnimations())
    {
        animation.RemoveAnimationTrackByPartId(id);
    }
}

const std::vector<CAnimation>& CAnimationEditor::GetAnimation() const
{
    return m_creature.GetAnimations();
}
