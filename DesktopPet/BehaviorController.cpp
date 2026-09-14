#include "Animation.h"
#include "BehaviorController.h"
#include "Creature.h"
#include "Genome.h"

#include <cmath>
#include <cstdlib>

using namespace Creature;
using namespace Creature::Animation;

namespace
{
    float Distance(
        float x1,
        float y1,
        float x2,
        float y2)
    {
        const float dx = x2 - x1;
        const float dy = y2 - y1;

        return std::sqrt(dx * dx + dy * dy);
    }
}

void CBehaviorController::Update(
    CCreature& creature,
    const InputState& input,
    float deltaTime)
{
    m_stateTime += deltaTime;

    const Genome& genome = creature.GetGenome();

    const float dx =
        input.mouseX - creature.GetPositionX();

    const float dy =
        input.mouseY - creature.GetPositionY();

    const float mouseDistance =
        std::sqrt(dx * dx + dy * dy);

    //
    // マウスによる割り込み
    //
    if (mouseDistance < 100.0f &&
        genome.mouseReaction == MouseReaction::Flee)
    {
        ChangeState(BehaviorState::FleeMouse);
    }

    switch (m_state)
    {
    case BehaviorState::Idle:
    {
        creature.SetVelocity(
            Direction::Horizontal,
            0.0f);

        creature.SetAnimationState(
            AnimationState::Idle);

        //
        // 2秒くらい止まったら歩く
        //
        if (m_stateTime >= 2.0f)
        {
            ChangeState(BehaviorState::Walk);
        }

        break;
    }

    case BehaviorState::Walk:
    {
        creature.SetVelocity(
            Direction::Horizontal,
            genome.speed);

        creature.SetAnimationState(
            AnimationState::Walk);

        //
        // 3秒歩いたら休む
        //
        if (m_stateTime >= 3.0f)
        {
            ChangeState(BehaviorState::Idle);
        }

        break;
    }

    case BehaviorState::FleeMouse:
    {
        //
        // マウスと逆方向に逃げる
        //
        const float direction =
            input.mouseX < creature.GetPositionX()
            ? 1.0f
            : -1.0f;

        creature.SetVelocity(
            Direction::Horizontal,
            genome.speed * 2.0f * direction);

        creature.SetAnimationState(
            AnimationState::Run);

        //
        // 十分離れたら通常状態へ
        //
        if (mouseDistance > 200.0f)
        {
            ChangeState(BehaviorState::Idle);
        }

        break;
    }
    }
}

void CBehaviorController::ChangeState(
    BehaviorState state)
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    m_stateTime = 0.0f;
}
