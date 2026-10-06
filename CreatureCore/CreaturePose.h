#pragma once

#include "Transform2D.h"

#include <vector>

namespace Creature
{
    class CSkeleton;

    /**
     * @brief Represents the complete visual pose of a creature at a specific point in time.
     */
    class CCreaturePose
    {
    public:
        static CCreaturePose Default(const CSkeleton& skeleton);
    public:
        std::vector<Math::CTransform2D>& GetPartTransform();
        const std::vector<Math::CTransform2D>& GetPartTransform() const;

        Math::CTransform2D& GetRootTransform();
        const Math::CTransform2D& GetRootTransform() const;
        void SetRootTransform(const Math::CTransform2D& transform);
    private:
        Math::CTransform2D m_rootTransform; // Root transform applied to the entire creature.

        std::vector<Math::CTransform2D> m_vecPartTransform; // Per-part transforms corresponding to the skeleton part indices.
    };
}
