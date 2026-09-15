#pragma once

namespace Creature
{
    namespace Math
    {
        struct Vec2;

        class CMatrix3x2
        {
        public:
            CMatrix3x2();
            CMatrix3x2(
                float fm11,
                float fm12,
                float fm21,
                float fm22,
                float fm31,
                float fm32);
        public:

            static CMatrix3x2 CreateTranslation(const Vec2& position);
            static CMatrix3x2 CreateScale(const Vec2& scale);
            static CMatrix3x2 CreateRotation(float rotation);

            Vec2 TransformPoint(const Vec2& point) const;
            bool TryInverse(CMatrix3x2& inverse) const;

            CMatrix3x2 operator*(const CMatrix3x2& rhs) const;
        private:
            float m_fM11 = 1.0f;
            float m_fM12 = 0.0f;

            float m_fM21 = 0.0f;
            float m_fM22 = 1.0f;

            float m_fM31 = 0.0f;
            float m_fM32 = 0.0f;
        };
    } // namespace Math
} // namespace Creature
