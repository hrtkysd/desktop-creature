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

        Part* FindPartByName(std::string_view name);
        const Part* FindPartByName(std::string_view name) const;

        Part* FindPartById(PartId id);
        const Part* FindPartById(PartId id) const;

        PartId FindPartIdByName(const std::string_view name) const;

        PartId AddPart(const std::string_view name, PartId parentId = INVALID_PART_ID);
    private:

        std::vector<Part> m_vecPart;
        PartId m_nextPartId = 0;
    };
};
