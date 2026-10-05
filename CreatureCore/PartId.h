#pragma once

#include <cstdint>
#include <limits>

namespace Creature
{
    using PartId = std::uint32_t;
    using PartIndex = std::uint32_t;

    inline constexpr PartId INVALID_PART_ID
        = std::numeric_limits<PartId>::max();
    inline constexpr PartIndex INVALID_PART_INDEX
        = std::numeric_limits<PartIndex>::max();
}
