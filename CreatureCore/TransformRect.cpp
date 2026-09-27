#include "pch.h"
#include "TransformRect.h"

using namespace Creature;
using namespace Creature::Math;

CTransformRect::CTransformRect() = default;

CTransformRect::CTransformRect(const Vec2& size, const CMatrix3x2& matrix)
    : m_size(size)
    , m_matrix(matrix)
{
}

const CMatrix3x2& CTransformRect::Matrix() const
{
    return m_matrix;
}

RectCorner CTransformRect::Corner() const
{
    const Vec2 half{
         m_size.x * 0.5f,
         m_size.y * 0.5f
    };

    return
    {
        m_matrix.TransformPoint({ -half.x, -half.y }),
        m_matrix.TransformPoint({  half.x, -half.y }),
        m_matrix.TransformPoint({  half.x,  half.y }),
        m_matrix.TransformPoint({ -half.x,  half.y })
    };
}

const Vec2& CTransformRect::Size() const
{
    return m_size;
}

bool CTransformRect::Contains(const Vec2& point) const
{
    CMatrix3x2 inverse;
    if (!m_matrix.TryInverse(inverse)) return false;

    const auto localPoint = inverse.TransformPoint(point);
    const Vec2 halfSize
    {
        m_size.x * 0.5f,
        m_size.y * 0.5f
    };

    return
        localPoint.x >= -halfSize.x &&
        localPoint.x <= halfSize.x &&
        localPoint.y >= -halfSize.y &&
        localPoint.y <= halfSize.y;
}
