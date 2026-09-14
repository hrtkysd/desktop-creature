#pragma once

#include "Part.h"

#include <string_view>
#include <vector>

namespace Creature
{
    class CSkeleton
    {
    public:
        const std::vector<Part>& Parts() const noexcept;
        std::vector<Part>& Parts() noexcept;

        const Part* FindPartByName(const std::string_view name) const;
        const Part* FindPartById(PartId id) const;

        PartId FindPartIdByName(const std::string_view& name) const;
    private:
        std::vector<Part> m_vecPart;
    };
};
