#pragma once

#include "IMotionEditContext.h"
#include "PartId.h"

#include <string_view>
#include <unordered_set>

namespace Creature
{
    class CSkeleton;

    struct Part;

    namespace Math
    {
        class CTransform2D;
        struct Vec2;
    }
    namespace Editor
    {
        class CCreatureEditor;

        class CSkeletonEditor
        {
            friend class CMotionEditor;
        public:
            CSkeletonEditor() = delete;
            explicit CSkeletonEditor(
                CSkeleton& skeleton,
                IMotionEditContext& context);

        public:
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

            Creature::PartId AddPart(
                const std::string_view name,
                Creature::PartId parentId = INVALID_PART_ID);
            Creature::PartId AddPart(Creature::Part&& part);
            bool AddPartWithId(Part&& part);

            bool RemovePart(Creature::PartId id);

            const CSkeleton& GetSkeleton() const;
        private:
            std::unordered_set<PartId> RemovePartCore(Creature::PartId id);

        private:
            CSkeleton& m_skeleton;
            IMotionEditContext& m_context;
        };
    }
}
