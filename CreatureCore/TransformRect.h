#pragma once

#include "Matrix3x2.h"
#include "RectCorner.h"
#include "Vec2.h"

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
                const CMatrix3x2& matrix);
        public:
            const CMatrix3x2& Matrix() const;
            RectCorner Corner() const;
            const Vec2& Size() const;
            bool Contains(const Vec2& point) const;
        private:
            Vec2 m_size{};
            CMatrix3x2 m_matrix;
        };
    } // namespace Math
} // namespace Creature
