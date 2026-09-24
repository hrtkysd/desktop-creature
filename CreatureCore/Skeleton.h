#pragma once

#include "Part.h"

#include <string_view>
#include <unordered_set>
#include <vector>

namespace Creature
{
    namespace Editor
    {
        class CSkeletonEditor;
    }

    namespace Math
    {
        class CTransform2D;
        struct Vec2;
    }

    class CSkeleton
    {
        friend class Editor::CSkeletonEditor;
    public:
        const std::vector<Part>& Parts() const noexcept;

        const Part* FindPartByName(std::string_view name) const;
        const Part* FindPartById(PartId id) const;

        PartId FindPartIdByName(const std::string_view name) const;
        int FindPartIndexById(PartId id) const;

        bool HasChildren(Creature::PartId id) const;

        std::vector<Creature::PartId> GetRootPartIds() const;
        std::vector<Creature::PartId> GetChildPartIds(Creature::PartId id) const;
    private:
        std::vector<Part>& Parts() noexcept;

        Part* FindPartByName(std::string_view name);
        Part* FindPartById(PartId id);

        bool SetPartTransform(
            Creature::PartId id,
            const Creature::Math::CTransform2D& transform);
        bool SetPartPivotAndTransform(
            Creature::PartId id,
            const Creature::Math::Vec2& pivot,
            const Creature::Math::CTransform2D& transform);
        bool SetPartRotation(
            PartId id,
            float rotation);
        bool SetPartPosition(
            Creature::PartId id,
            const Creature::Math::Vec2& position);
        bool SetPartScale(
            Creature::PartId id,
            const Creature::Math::Vec2& scale);
        bool SetPartPivot(
            Creature::PartId id,
            const Creature::Math::Vec2& pivot);

        PartId AddPart(const std::string_view name, PartId parentId = INVALID_PART_ID);
        PartId AddPart(Part&& part);
        bool AddPartWithId(Part&& part);
        std::unordered_set<PartId> RemovePart(PartId id);
    private:
        void CollectDescendants(PartId parentId, std::unordered_set<PartId>& ids) const;
    private:
        std::vector<Part> m_vecPart;
        PartId m_nextPartId = 0;
    };
};
