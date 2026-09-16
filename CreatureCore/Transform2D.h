#pragma once

#include "Matrix3x2.h"
#include "Vec2.h"

namespace Creature
{
    namespace Math
    {
        class CTransform2D
        {
        public:
            CTransform2D();
            CTransform2D(const Vec2& position, const Vec2& scale, float fRotation);
        public:
            Vec2& GetPosition();
            const Vec2& GetPosition() const;
            void SetPosition(const Vec2& position);

            Vec2& GetScale();
            const Vec2& GetScale() const;
            void SetScale(const Vec2& scale);

            float GetRotation() const;
            void SetRotation(float fRotation);

            CMatrix3x2 ToMatrix() const;
        private:
            Vec2 m_position{ 0.0f, 0.0f };
            Vec2 m_scale{ 1.0f, 1.0f };
            float m_fRotation = 0.0f;
        };
    } // namespace Math
} // namespace Creature
