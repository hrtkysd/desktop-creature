#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

Creature::Editor::CAnimationEditor::CAnimationEditor(std::vector<CAnimation>& vecAnimation)
    : m_vecAnimation(vecAnimation)
{
}

bool CAnimationEditor::SetName(
    AnimationId id,
    const std::string& strName)
{
    if (auto animation = FindAnimationById(id))
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
    if (auto animation = FindAnimationById(id))
    {
        return animation->SetDuration(fDuration);
    }
    return false;
}

bool CAnimationEditor::AddTrack(
    AnimationId id,
    CAnimationTrack&& animationTrack)
{
    if (auto animation = FindAnimationById(id))
    {
        return animation->AddAnimationTrack(std::move(animationTrack));
    }
    return false;
}

bool CAnimationEditor::RemoveTrack(
    AnimationId id,
    const CAnimationTrackKey& trackKey)
{
    if (auto animation = FindAnimationById(id))
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
    if (auto animation = FindAnimationById(id))
    {
        return animation->AddOrUpdateKeyFrame(trackKey, keyFrame);
    }
    return false;
}

void CAnimationEditor::RemovePart(PartId id)
{
    for (auto& animation : m_vecAnimation)
    {
        animation.RemoveAnimationTrackByPartId(id);
    }
}

const std::vector<CAnimation>& CAnimationEditor::GetAnimations() const
{
    return m_vecAnimation;
}

CAnimation* CAnimationEditor::FindAnimationById(AnimationId id)
{
    const auto it = std::find_if(
        m_vecAnimation.begin(),
        m_vecAnimation.end(),
        [id](const CAnimation& animation)
        {
            return animation.GetAnimationId() == id;
        });

    return it != m_vecAnimation.end()
        ? std::addressof(*it)
        : nullptr;
}
