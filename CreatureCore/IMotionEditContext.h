#pragma once

#include "PartId.h"

namespace Creature
{
    namespace Editor
    {
        class IMotionEditContext
        {
        public:
            virtual ~IMotionEditContext() = default;
            virtual bool RemovePart(PartId id) = 0;
        };
    }
}
