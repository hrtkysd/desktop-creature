#pragma once

#include "Transform2D.h"
#include "Vec2.h"

#include <cstdint>
#include <string>

namespace Creature
{
    using PartId = std::uint32_t;
    using PartSlot = std::size_t;

    inline constexpr PartId INVALID_PART_ID = std::numeric_limits<PartId>::max();
    inline constexpr uint32_t INVALID_PART_INDEX = std::numeric_limits<PartId>::max();

    /**
     * @brief Represents creature's part properties.
     */
    struct Part
    {
        PartId id = INVALID_PART_ID; // Stable part identifier for this part.
        std::string strName;    // parts name. e.g. "Leg", "Tail"....
        PartId parentId = INVALID_PART_ID; // Parent part identifier.

        Math::CTransform2D bindTransform{}; // bind transform relative to the parent
        Math::Vec2 pivot{};     // Pivot point used for rotation and scaling.
    };
}
