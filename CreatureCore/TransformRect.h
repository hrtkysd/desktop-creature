#pragma once

#include "Matrix3x2.h"
#include "RectCorner.h"
#include "Vec2.h"

#include <array>

namespace Creature
{
    namespace Math
    {
        class CTransformRect
        {
        public:
            CTransformRect();
            explicit CTransformRect(
                const Vec2& size,
                const CMatrix3x2& transform);
        public:
            RectCorner Corner() const;
            bool Contains(const Vec2& point) const;
        private:
            Vec2 m_size{};
            CMatrix3x2 m_transform;
        };
    } // namespace Math
} // namespace Creature
