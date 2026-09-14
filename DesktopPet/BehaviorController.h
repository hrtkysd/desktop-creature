#pragma once

namespace Creature
{
    class CCreature;
}

struct InputState
{
    float mouseX = 0.0f;
    float mouseY = 0.0f;

    bool leftButtonDown = false;
};

enum class BehaviorState
{
    Idle,
    Walk,
    WatchMouse,
    ChaseMouse,
    FleeMouse
};

class CBehaviorController
{
public:
    void Update(
        Creature::CCreature& creature,
        const InputState& input,
        float deltaTime);

private:
    void ChangeState(BehaviorState state);

private:
    BehaviorState m_state = BehaviorState::Idle;
    float m_stateTime = 0.0f;
};
