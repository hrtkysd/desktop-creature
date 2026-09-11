#pragma once

#include <memory>

struct CreaturePose;

enum class AnimationState
{
    Idle,
    Walk,
    Run,
    Sleep
};

class CAnimation
{
public:
    CAnimation();
public:
    void Update(
        float deltaTime,
        AnimationState state,
        float velocityX);

    const CreaturePose& GetPose() const noexcept
    {
        return *m_pose;
    }

private:
    float m_time = 0.0f;
    float m_blinkTimer = 0.0f;

    std::unique_ptr<CreaturePose> m_pose{};
};