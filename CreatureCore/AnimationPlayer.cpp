#include "pch.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "AnimationTrack.h"
#include "CreaturePose.h"
#include "Skeleton.h"

#include <cmath>

using namespace Creature;
using namespace Creature::Animation;

void CAnimationPlayer::Play()
{
    if (m_eAnimationPlayerState == AnimationPlayerState::Stop)
    {
        m_fCurrentTime = 0.0f;
    }
    m_eAnimationPlayerState = AnimationPlayerState::Play;
}

void CAnimationPlayer::Stop()
{
    m_fCurrentTime = 0.0f;
    m_eAnimationPlayerState = AnimationPlayerState::Stop;
}

void CAnimationPlayer::Pause()
{
    if (m_eAnimationPlayerState != AnimationPlayerState::Play) return;
    m_eAnimationPlayerState = AnimationPlayerState::Pause;
}

void CAnimationPlayer::Update(float fDeltaTime)
{
    if (m_eAnimationPlayerState != AnimationPlayerState::Play) return;
    m_fCurrentTime += fDeltaTime;
}

const CreaturePose& CAnimationPlayer::GetPose() const noexcept
{
    return m_pose;
}

void CAnimationPlayer::SamplePose(const CAnimation& animation, const CSkeleton& skeleton)
{
    if (animation.GetDuration() > 0.0f)
    {
        m_fCurrentTime = std::fmod(m_fCurrentTime, animation.GetDuration());
    }

    m_pose.vecPartTransform.clear();
    m_pose.vecPartTransform.resize(skeleton.Parts().size());

    for (const auto& track : animation.GetAnimationTrack())
    {
        const auto index = skeleton.FindPartIndexById(track.GetPartId());
        if (index == INVALID_PART_INDEX) continue;

        const auto value = track.Sample(m_fCurrentTime);
        auto& transform = m_pose.vecPartTransform.at(index);

        switch (track.GetAnimationProperty())
        {
        case AnimationProperty::PositionX:
            transform.GetPosition().x = value;
            break;

        case AnimationProperty::PositionY:
            transform.GetPosition().y = value;
            break;

        case AnimationProperty::Rotation:
            transform.SetRotation(value);
            break;

        case AnimationProperty::ScaleX:
            transform.GetScale().x = value;
            break;

        case AnimationProperty::ScaleY:
            transform.GetScale().y = value;
            break;
        }
    }
}
