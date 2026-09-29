#include "pch.h"
#include "BehaviorController.h"
#include "Creature.h"

#include <cmath>

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
    // TBD
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
