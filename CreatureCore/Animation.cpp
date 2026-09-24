#include "pch.h"
#include "Animation.h"
#include "AnimationTrack.h"

#include <algorithm>

using namespace Creature;
using namespace Creature::Animation;

CAnimation::CAnimation(AnimationId id, const std::string& strName)
    : CAnimation(id, strName, 0.0f, {})
{
}

CAnimation::CAnimation(
    AnimationId id,
    const std::string& strName,
    float fDuration,
    const std::vector<CAnimationTrack>& vecAnimationTrack)
    : m_animationId(id)
    , m_strName(strName)
    , m_fDuration(fDuration)
    , m_vecAnimationTrack(vecAnimationTrack)
{
}

CAnimation::~CAnimation() = default;

AnimationId CAnimation::GetAnimationId() const
{
    return m_animationId;
}

const std::string& CAnimation::GetName() const
{
    return m_strName;
}

float CAnimation::GetDuration() const
{
    return m_fDuration;
}

const std::vector<CAnimationTrack>& CAnimation::GetAnimationTracks() const
{
    return m_vecAnimationTrack;
}

const CAnimationTrack* CAnimation::FindAnimationTrack(
    const CAnimationTrackKey& key) const
{
    const auto itFind = std::find_if(m_vecAnimationTrack.begin(), m_vecAnimationTrack.end(), [&key](const CAnimationTrack& track)
        {
            return track.Matches(key);
        });
    return itFind != m_vecAnimationTrack.end()
        ? std::addressof(*itFind)
        : nullptr;
}

CAnimation CAnimation::NewAnimation(AnimationId id, const std::string& strName)
{
    return CAnimation(id, strName);
}

void CAnimation::SetAnimationId(AnimationId id)
{
    m_animationId = id;
}

void CAnimation::SetName(const std::string& strName)
{
    m_strName = strName;
}

bool CAnimation::SetDuration(float fDuration)
{
    if (fDuration < 0.0f) return false;

    const auto hasOutOfRangeKeyFrame = std::any_of(m_vecAnimationTrack.cbegin(), m_vecAnimationTrack.cend()
        , [fDuration](const CAnimationTrack& track)
        {
            const auto& keyFrames = track.GetKeyFrames();
            return !keyFrames.empty() && keyFrames.back().fTime > fDuration;
        });
    if (hasOutOfRangeKeyFrame) return false;

    m_fDuration = fDuration;
    return true;
}

bool CAnimation::AddOrUpdateKeyFrame(const CAnimationTrackKey& key, const FloatKeyFrame& keyFrame)
{
    if (keyFrame.fTime < 0.0f || keyFrame.fTime > m_fDuration)
    {
        return false;
    }

    if (auto track = FindAnimationTrack(key))
    {
        track->AddOrUpdateKeyFrame(keyFrame);
        return true;
    }
    return false;
}

bool CAnimation::AddAnimationTrack(CAnimationTrack&& animationTrack)
{
    if (!animationTrack.GetKey().IsValid()) return false;

    if (FindAnimationTrack(animationTrack.GetKey())) return false;

    const auto& keyFrames = animationTrack.GetKeyFrames();
    if (!keyFrames.empty() && keyFrames.back().fTime > m_fDuration)
    {
        return false;
    }

    m_vecAnimationTrack.emplace_back(std::move(animationTrack));
    return true;
}

bool CAnimation::RemoveAnimationTrackByKey(const CAnimationTrackKey& key)
{
    const auto it = std::remove_if(
        m_vecAnimationTrack.begin(),
        m_vecAnimationTrack.end(),
        [&key](const CAnimationTrack& track)
        {
            return track.Matches(key);
        });

    if (it == m_vecAnimationTrack.end()) return false;

    m_vecAnimationTrack.erase(it);
    return true;
}

bool CAnimation::RemoveAnimationTrackByPartId(PartId id)
{
    const auto it = std::remove_if(
        m_vecAnimationTrack.begin(),
        m_vecAnimationTrack.end(),
        [id](const CAnimationTrack& track)
        {
            return track.GetKey().GetPartId() == id;
        });

    if (it == m_vecAnimationTrack.end()) return false;

    m_vecAnimationTrack.erase(it, m_vecAnimationTrack.end());
    return true;
}

CAnimationTrack* CAnimation::FindAnimationTrack(const CAnimationTrackKey& key)
{
    return const_cast<CAnimationTrack*>(std::as_const(*this).FindAnimationTrack(key));
}
