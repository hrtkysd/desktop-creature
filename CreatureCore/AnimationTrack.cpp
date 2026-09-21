#include "pch.h"
#include "AnimationTrack.h"
#include "MathUtils.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Math;

CAnimationTrack::CAnimationTrack(const CAnimationTrackKey& key)
    : m_animationTrackKey(key)
{
}

float CAnimationTrack::Sample(float fTime) const
{
    if (m_vecKeyFrame.empty()) return 0.0f;

    if (fTime <= m_vecKeyFrame.front().fTime) return m_vecKeyFrame.front().fValue;
    if (fTime >= m_vecKeyFrame.back().fTime) return m_vecKeyFrame.back().fValue;

    for (std::size_t i = 0; i + 1 < m_vecKeyFrame.size(); ++i)
    {
        const auto& from = m_vecKeyFrame.at(i);
        const auto& to = m_vecKeyFrame.at(i + 1);

        if (fTime < from.fTime || fTime > to.fTime)  continue;

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

    return 0.0f;
}

const CAnimationTrackKey& CAnimationTrack::GetKey() const noexcept
{
    return m_animationTrackKey;
}

const std::vector<FloatKeyFrame>& CAnimationTrack::GetKeyFrames() const
{
    return m_vecKeyFrame;
}

void CAnimationTrack::AddOrUpdateKeyFrame(const FloatKeyFrame& keyFrame)
{
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
        return;
    }

    m_vecKeyFrame.insert(it, keyFrame);
}

bool CAnimationTrack::Matches(const CAnimationTrackKey& key) const noexcept
{
    return m_animationTrackKey == key;
}

