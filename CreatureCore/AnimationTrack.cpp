#include "pch.h"
#include "AnimationTrack.h"
#include "Math.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Math;

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
        const float t =(fTime - from.fTime) / (to.fTime - from.fTime);
        float fInterpolation = t;

        switch (m_eInterpolation)
        {
        case Interpolation::Linear:
            break;
        case Interpolation::SmoothStep:
            fInterpolation = t * t * (3.0f - 2.0f * t);
            break;
        case Interpolation::Step:
            return from.fValue;
        }
        return Lerp(from.fValue, to.fValue, fInterpolation);
    }

    return 0.0f;
}

PartId CAnimationTrack::GetPartId() const
{
    return m_targetPartId;
}

void CAnimationTrack::SetPartId(PartId id)
{
    m_targetPartId = id;
}

AnimationProperty CAnimationTrack::GetAnimationProperty() const
{
    return m_eProperty;
}

void CAnimationTrack::SetAnimationProperty(AnimationProperty eProperty)
{
    m_eProperty = eProperty;
}

Interpolation Creature::Animation::CAnimationTrack::GetInterpolation() const noexcept
{
    return m_eInterpolation;
}

void CAnimationTrack::SetInterpolation(Interpolation eInterpolation) noexcept
{
    m_eInterpolation = eInterpolation;
}

const std::vector<FloatKeyFrame>& CAnimationTrack::GetKeyFrames() const
{
    return m_vecKeyFrame;
}

void CAnimationTrack::AddKeyFrame(FloatKeyFrame&& keyFrame)
{
    m_vecKeyFrame.emplace_back(std::move(keyFrame));
}
