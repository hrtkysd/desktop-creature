#pragma once

#include "PartId.h"
#include "Vec2.h"

#include <cstdint>

enum class ResizeHandle : std::uint8_t
{
    None,
    TopLeft,
    TopRight,
    BottomRight,
    BottomLeft
};

struct ResizeState
{
    ResizeHandle eHandle = ResizeHandle::None;
    Creature::PartId partId = Creature::INVALID_PART_ID;

    Creature::Math::Vec2 anchor{};
};
