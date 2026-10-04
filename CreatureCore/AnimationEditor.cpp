#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

CAnimationEditor::CAnimationEditor(CAnimation& animation)
    : m_animation(animation)
{
}

bool CAnimationEditor::SetDuration(float fDuration)
{
    return m_animation.SetDuration(fDuration);
}

bool CAnimationEditor::AddTrack(CAnimationTrack&& animationTrack)
{
    return m_animation.AddAnimationTrack(std::move(animationTrack));
}

bool CAnimationEditor::RemoveTrack(const CAnimationTrackKey& trackKey)
{
    return m_animation.RemoveAnimationTrackByKey(trackKey);
}

bool CAnimationEditor::AddOrUpdateKeyFrame(
    const CAnimationTrackKey& trackKey,
    const FloatKeyFrame& keyFrame)
{
    return m_animation.AddOrUpdateKeyFrame(trackKey, keyFrame);
}

void CAnimationEditor::RemovePart(PartId id)
{
    m_animation.RemoveAnimationTrackByPartId(id);
}

const CAnimation& CAnimationEditor::GetAnimation() const
{
    return m_animation;
}
