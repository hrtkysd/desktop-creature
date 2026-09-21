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

const std::vector<CAnimationTrack>& CAnimation::GetAnimationTrack() const
{
    return m_vecAnimationTrack;
}

void CAnimation::AddAnimationTrack(CAnimationTrack&& animationTrack)
{
    m_vecAnimationTrack.emplace_back(std::move(animationTrack));
}
