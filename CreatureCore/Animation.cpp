#include "pch.h"
#include "Animation.h"
#include "AnimationTrack.h"

using namespace Creature;
using namespace Creature::Animation;

CAnimation::CAnimation() = default;

CAnimation::CAnimation(const std::string& strName, float fDuration, const std::vector<CAnimationTrack>& vecAnimationTrack)
    : m_strName(strName)
    , m_fDuration(fDuration)
    , m_vecAnimationTrack(vecAnimationTrack)
{
}

CAnimation::~CAnimation() = default;

const std::string& CAnimation::GetName() const
{
    return m_strName;
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
