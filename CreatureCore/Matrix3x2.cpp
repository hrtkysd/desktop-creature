#include "pch.h"
#include "Matrix3x2.h"
#include "Vec2.h"

#include <cmath>

using namespace Creature;
using namespace Creature::Math;

CMatrix3x2::CMatrix3x2() = default;

CMatrix3x2::CMatrix3x2(
    float fM11,
    float fM12,
    float fM21,
    float fM22,
    float fM31,
    float fM32)
    : m_fM11(fM11)
    , m_fM12(fM12)
    , m_fM21(fM21)
    , m_fM22(fM22)
    , m_fM31(fM31)
    , m_fM32(fM32)
{
}

CMatrix3x2 CMatrix3x2::CreateTranslation(const Vec2& position)
{
    CMatrix3x2 result;

    result.m_fM31 = position.x;
    result.m_fM32 = position.y;

    return result;
}

CMatrix3x2 CMatrix3x2::CreateScale(const Vec2& scale)
{
    CMatrix3x2 result;

    result.m_fM11 = scale.x;
    result.m_fM22 = scale.y;

    return result;
}

CMatrix3x2 CMatrix3x2::CreateRotation(float rotation)
{
    const float c = std::cos(rotation);
    const float s = std::sin(rotation);

    CMatrix3x2 result;

    result.m_fM11 = c;
    result.m_fM12 = s;
    result.m_fM21 = -s;
    result.m_fM22 = c;

    return result;
}

Vec2 CMatrix3x2::TransformPoint(const Vec2& point) const
{
    return
    {
        point.x * m_fM11 + point.y * m_fM21 + m_fM31,
        point.x * m_fM12 + point.y * m_fM22 + m_fM32
    };
}

bool CMatrix3x2::TryInverse(CMatrix3x2& inverse) const
{
    const float determinant = m_fM11 * m_fM22 - m_fM12 * m_fM21;

    constexpr float EPSILON = 1.0e-6f;

    if (std::abs(determinant) < EPSILON) return false;

    const float invDet = 1.0f / determinant;

    inverse.m_fM11 = m_fM22 * invDet;
    inverse.m_fM12 = -m_fM12 * invDet;
    inverse.m_fM21 = -m_fM21 * invDet;
    inverse.m_fM22 = m_fM11 * invDet;

    inverse.m_fM31 = -(m_fM31 * inverse.m_fM11 + m_fM32 * inverse.m_fM21);
    inverse.m_fM32 = -(m_fM31 * inverse.m_fM12 + m_fM32 * inverse.m_fM22);

    return true;
}

CMatrix3x2 CMatrix3x2::operator*(const CMatrix3x2& rhs) const
{
    CMatrix3x2 result;

    result.m_fM11 =
        m_fM11 * rhs.m_fM11 +
        m_fM12 * rhs.m_fM21;

    result.m_fM12 =
        m_fM11 * rhs.m_fM12 +
        m_fM12 * rhs.m_fM22;

    result.m_fM21 =
        m_fM21 * rhs.m_fM11 +
        m_fM22 * rhs.m_fM21;

    result.m_fM22 =
        m_fM21 * rhs.m_fM12 +
        m_fM22 * rhs.m_fM22;

    result.m_fM31 =
        m_fM31 * rhs.m_fM11 +
        m_fM32 * rhs.m_fM21 +
        rhs.m_fM31;

    result.m_fM32 =
        m_fM31 * rhs.m_fM12 +
        m_fM32 * rhs.m_fM22 +
        rhs.m_fM32;

    return result;
}
