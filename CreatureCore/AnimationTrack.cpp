#include "pch.h"
#include "AnimationTrack.h"
#include "MathUtils.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Math;

namespace
{
    bool IsValidKeyFrame(const FloatKeyFrame& keyFrame)
    {
        return std::isfinite(keyFrame.fTime)
            && keyFrame.fTime >= 0.0f
            && std::isfinite(keyFrame.fValue);
    }
}

CAnimationTrack::CAnimationTrack(const CAnimationTrackKey& key)
    : m_animationTrackKey(key)
{
}

float CAnimationTrack::Sample(float fTime) const
{
    if (m_vecKeyFrame.empty()) return 0.0f;

    const auto it = std::lower_bound(
        m_vecKeyFrame.begin(),
        m_vecKeyFrame.end(),
        fTime,
        [](const FloatKeyFrame& keyFrame, float time)
        {
            return keyFrame.fTime < time;
        });

    if (it == m_vecKeyFrame.end()) return m_vecKeyFrame.back().fValue;
    if (it->fTime == fTime) return it->fValue;
    if (it == m_vecKeyFrame.begin()) return it->fValue;

    const auto& to = *it;
    const auto& from = *std::prev(it);

    auto t = (fTime - from.fTime) / (to.fTime - from.fTime);

    switch (from.eInterpolationToNext)
    {
    case Interpolation::Linear:
        break;
    case Interpolation::SmoothStep:
        t = Math::SmoothStep(t);
        break;
    case Interpolation::Step:
        return from.fValue;
    }
    return Math::Lerp(from.fValue, to.fValue, t);
}

const CAnimationTrackKey& CAnimationTrack::GetKey() const noexcept
{
    return m_animationTrackKey;
}

const std::vector<FloatKeyFrame>& CAnimationTrack::GetKeyFrames() const
{
    return m_vecKeyFrame;
}

bool CAnimationTrack::AddOrUpdateKeyFrame(const FloatKeyFrame& keyFrame)
{
    if (!IsValidKeyFrame(keyFrame)) return false;

    const auto it = std::lower_bound(
        m_vecKeyFrame.begin(),
        m_vecKeyFrame.end(),
        keyFrame.fTime,
        [](const FloatKeyFrame& key, float time)
        {
            return key.fTime < time;
        });

    if (it != m_vecKeyFrame.cend() && it->fTime == keyFrame.fTime)
    {
        *it = keyFrame;
        return true;
    }

    m_vecKeyFrame.insert(it, keyFrame);
    return true;
}

bool CAnimationTrack::IsValid() const
{
    if (!m_animationTrackKey.IsValid()) return false;
    return std::all_of(
        m_vecKeyFrame.cbegin(),
        m_vecKeyFrame.cend(),
        IsValidKeyFrame);
}

bool CAnimationTrack::Matches(const CAnimationTrackKey& key) const noexcept
{
    return m_animationTrackKey == key;
}

