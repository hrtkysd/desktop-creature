#pragma once

#include "Part.h"

#include <string_view>
#include <vector>

namespace Creature
{
    class CSkeleton
    {
    public:
        const std::vector<Creature::Part>& Parts() const noexcept;
        std::vector<Creature::Part>& Parts() noexcept;

        int FindPartIndex(const std::string_view& name) const;

    private:
        std::vector<Creature::Part> m_vecPart;
    };
};
