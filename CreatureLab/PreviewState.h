#pragma once

#include "Part.h"

struct PreviewState
{
    float fZoom = 0.25f;
    Creature::PartId eSelectPartId = Creature::INVALID_PART_ID;
};
