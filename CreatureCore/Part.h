#pragma once

#include "PartId.h"
#include "Transform2D.h"
#include "Vec2.h"

#include <string>

namespace Creature
{
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
