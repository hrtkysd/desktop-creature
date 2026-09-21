#pragma once

#include "Part.h"

#include <string_view>
#include <unordered_set>
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

        int FindPartIndexById(PartId id) const;

        PartId AddPart(const std::string_view name, PartId parentId = INVALID_PART_ID);
        PartId AddPart(Part&& part);

        bool AddPartWithId(Part&& part);
        bool RemovePart(PartId id);

        bool HasChildren(Creature::PartId id) const;
        std::vector<Creature::PartId> GetRootPartIds() const;
        std::vector<Creature::PartId> GetChildPartIds(Creature::PartId id) const;
    private:
        void CollectDescendants(PartId parentId, std::unordered_set<PartId>& ids) const;
    private:
        std::vector<Part> m_vecPart;
        PartId m_nextPartId = 0;
    };
};
