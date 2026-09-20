#pragma once

#include "PartId.h"

#include <cstdint>

enum class OperationType : std::uint8_t
{
    None,
    Move,
    Resize,
    Rotate,
    Pivot,
};

struct Operation
{
    OperationType eType = OperationType::None;
    Creature::PartId partId = Creature::INVALID_PART_ID;
};
