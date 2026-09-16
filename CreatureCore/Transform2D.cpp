#include "pch.h"
#include "Transform2D.h"

using namespace Creature;
using namespace Creature::Math;

CTransform2D::CTransform2D() = default;

CTransform2D::CTransform2D(const Vec2& position, const Vec2& scale, float fRotation)
    : m_position(position)
    , m_scale(scale)
    , m_fRotation(fRotation)
{
}

Vec2& CTransform2D::GetPosition()
{
    return m_position;
}

const Vec2& CTransform2D::GetPosition() const
{
    return m_position;
}

void CTransform2D::SetPosition(const Vec2& position)
{
    m_position = position;
}

Vec2& CTransform2D::GetScale()
{
    return m_scale;
}

const Vec2& CTransform2D::GetScale() const
{
    return m_scale;
}

void CTransform2D::SetScale(const Vec2& scale)
{
    m_scale = scale;
}

float CTransform2D::GetRotation() const
{
    return m_fRotation;
}

void CTransform2D::SetRotation(float fRotation)
{
    m_fRotation = fRotation;
}

CMatrix3x2 CTransform2D::ToMatrix() const
{
    return
        CMatrix3x2::CreateScale(m_scale) *
        CMatrix3x2::CreateRotation(m_fRotation) *
        CMatrix3x2::CreateTranslation(m_position);
}
