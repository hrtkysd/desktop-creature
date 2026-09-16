#include "pch.h"
#include "AnimationTrack.h"

using namespace Creature;
using namespace Creature::Animation;

namespace
{
    float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }
}

float CAnimationTrack::Sample(float fTime) const
{
    if (m_vecKeyFrame.empty()) return 0.0f;

    if (fTime <= m_vecKeyFrame.front().fTime) return m_vecKeyFrame.front().fValue;
    if (fTime >= m_vecKeyFrame.back().fTime) return m_vecKeyFrame.back().fValue;

    for (std::size_t i = 0; i + 1 < m_vecKeyFrame.size(); ++i)
    {
        const auto& key0 = m_vecKeyFrame.at(i);
        const auto& key1 = m_vecKeyFrame.at(i + 1);

        if (fTime < key0.fTime || fTime > key1.fTime) continue;
        const float t =(fTime - key0.fTime) / (key1.fTime - key0.fTime);

        return Lerp(key0.fValue, key1.fValue, t);
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

const std::vector<FloatKeyFrame>& CAnimationTrack::GetKeyFrames() const
{
    return m_vecKeyFrame;
}

void CAnimationTrack::AddKeyFrame(FloatKeyFrame&& keyFrame)
{
    m_vecKeyFrame.emplace_back(std::move(keyFrame));
}
