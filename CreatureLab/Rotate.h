#pragma once

#include "Vec2.h"

struct RotateState
{
    float fPartRotationStart = 0.0f;
    float fMouseAngleStart = 0.0f;
    Creature::Math::Vec2 pivot{};
};
