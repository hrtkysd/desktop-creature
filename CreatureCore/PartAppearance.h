#pragma once

#include "ZOrder.h"
#include "PartId.h"

#include <filesystem>

namespace Creature
{
    struct PartAppearance
    {
        PartId partId = INVALID_PART_ID;
        std::filesystem::path texturePath;
        ZOrder zOrder = MIN_Z_ORDER;
    };
}
