#pragma once

#include "Transform2D.h"
#include "Vec2.h"

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

        Math::Vec2& GetEyeOffset();
        const Math::Vec2& GetEyeOffset() const;
        void SetEyeOffset(const Math::Vec2& offset);

        bool IsBlink() const;
        void SetBlink(bool isBlink);
    private:
        Math::CTransform2D m_rootTransform; // Root transform applied to the entire creature.
        Math::Vec2 m_eyeOffset{}; // Offset applied to the eyes for gaze movement.

        bool m_isBlink = false; // Indicates whether the creature is currently blinking.

        std::vector<Math::CTransform2D> m_vecPartTransform; // Per-part transforms corresponding to the skeleton part indices.
    };
}
