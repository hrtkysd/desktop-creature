#pragma once

#include "Transform2D.h"
#include "Vec2.h"

#include <string>

namespace Creature
{
    struct Part
    {
        std::string strName;    // parts name. ex): Leg, Tail....
        int iParent = -1;       // parent parts in skelton, root=-1.

        Math::Transform2D bindTransform{}; // standard attitudes towards parents.
        Math::Vec2 pivot{};     // rotate central position of texture.
    };
}
