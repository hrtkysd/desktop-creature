#pragma once

#include "Part.h"

namespace Creature
{
    struct Appearance
    {
        PartId partId = INVALID_PART_ID;
        std::string strTexturePath;
    };
}
