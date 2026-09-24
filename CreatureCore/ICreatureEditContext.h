#pragma once

#include "PartId.h"

namespace Creature
{
    namespace Editor
    {
        class ICreatureEditContext
        {
        public:
            virtual ~ICreatureEditContext() = default;
            virtual bool RemovePart(PartId id) = 0;
        };
    }
}
