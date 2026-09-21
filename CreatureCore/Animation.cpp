#include "pch.h"
#include "Animation.h"

using namespace Creature;
using namespace Creature::Animation;

CAnimation::CAnimation(AnimationId id, const std::string& strName)
    : CAnimation(id, strName, 0.0f, {})
{
}

CAnimation::CAnimation(AnimationId id, const std::string& strName, float fDuration, const std::vector<CAnimationTrack>& vecAnimationTrack)
    : m_animationId(id)
    , m_strName(strName)
    , m_fDuration(fDuration)
    , m_vecAnimationTrack(vecAnimationTrack)
{
}

CAnimation::~CAnimation() = default;

CAnimation CAnimation::NewAnimation(AnimationId id, const std::string& strName)
{
    return CAnimation(id, strName);
}

void CAnimation::SetAnimationId(AnimationId id)
{
    m_animationId = id;
}

AnimationId CAnimation::GetAnimationId() const
{
    return m_animationId;
}

const std::string& CAnimation::GetName() const
{
    return m_strName;
}

void CAnimation::SetName(const std::string& strName)
{
    m_strName = strName;
}

float CAnimation::GetDuration() const
{
    return m_fDuration;
}

void CAnimation::SetDuration(float fDuration)
{
    m_fDuration = fDuration;
}

const std::vector<CAnimationTrack>& CAnimation::GetAnimationTracks() const
{
    return m_vecAnimationTrack;
}

bool CAnimation::AddAnimationTrack(CAnimationTrack&& animationTrack)
{
    const auto itFind = std::find_if(m_vecAnimationTrack.begin(), m_vecAnimationTrack.end(), [&animationTrack](const CAnimationTrack& track)
        {
            return animationTrack.Matches(track.GetKey());
        });

    if (itFind != m_vecAnimationTrack.end()) return false;

    m_vecAnimationTrack.emplace_back(std::move(animationTrack));
    return true;
}

bool CAnimation::RemoveAnimationTrack(const CAnimationTrackKey& key)
{
    const auto itFind = std::find_if(m_vecAnimationTrack.begin(), m_vecAnimationTrack.end(), [&key](const CAnimationTrack& track)
        {
            return track.Matches(key);
        });
    if (itFind == m_vecAnimationTrack.end()) return false;
    m_vecAnimationTrack.erase(itFind);
    return true;
}

CAnimationTrack* CAnimation::FindAnimationTrack(const CAnimationTrackKey& key)
{
    return const_cast<CAnimationTrack*>(std::as_const(*this).FindAnimationTrack(key));
}

const CAnimationTrack* CAnimation::FindAnimationTrack(const CAnimationTrackKey& key) const
{
    const auto itFind = std::find_if(m_vecAnimationTrack.begin(), m_vecAnimationTrack.end(), [&key](const CAnimationTrack& track)
        {
            return track.Matches(key);
        });
    return itFind != m_vecAnimationTrack.end()
        ? &(*itFind)
        : nullptr;
}
