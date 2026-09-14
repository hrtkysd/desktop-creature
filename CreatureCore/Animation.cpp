#include "pch.h"
#include "Animation.h"
#include "CreaturePose.h"
#include <cmath>

using namespace Creature;
using namespace Animation;

CAnimation::CAnimation()
    : m_pose(std::make_unique<CreaturePose>())
{
}

void CAnimation::Update(float deltaTime, AnimationState state, float velocityX)
{
    m_time += deltaTime;

    // 毎フレーム初期姿勢に戻す
    m_pose = std::make_unique<CreaturePose>();

    switch (state)
    {
    case AnimationState::Idle:
    {
        const float breathe =
            std::sin(m_time * 2.0f);

        m_pose->scaleX =
            1.0f + breathe * 0.02f;

        m_pose->scaleY =
            1.0f - breathe * 0.03f;

        break;
    }

    case AnimationState::Walk:
    {
        const float phase =
            m_time * 12.0f;

        m_pose->offsetY =
            -std::abs(std::sin(phase)) * 3.0f;

        const float squash =
            std::sin(phase) * 0.05f;

        m_pose->scaleX = 1.0f + squash;
        m_pose->scaleY = 1.0f - squash;

        break;
    }

    case AnimationState::Run:
    {
        const float phase =
            m_time * 18.0f;

        m_pose->offsetY =
            -std::abs(std::sin(phase)) * 5.0f;

        break;
    }

    case AnimationState::Sleep:
        // 後で実装
        break;
    }

    m_pose->blink =
        std::fmod(m_time, 4.0f) > 3.85f;
}

const Creature::CreaturePose& Creature::Animation::CAnimation::GetPose() const noexcept
{
    return *m_pose;
}
