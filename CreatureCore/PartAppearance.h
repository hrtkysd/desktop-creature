#pragma once

#include "PartId.h"

#include <filesystem>

namespace Creature
{
    struct PartAppearance
    {
        PartId partId = INVALID_PART_ID;
        std::filesystem::path texturePath;
    };
}
