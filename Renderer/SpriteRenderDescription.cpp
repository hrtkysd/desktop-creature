#include "pch.h"
#include "SpriteRenderDescription.h"
#include "Texture.h"
#include "Transform2D.h"

using namespace Creature::Math;

CSpriteRenderDescription::CSpriteRenderDescription(
    std::shared_ptr<const CTexture> texture,
    const Creature::Math::Vec2& size,
    const CTransform2D& transform,
    const Vec2& pivot)
    : CSpriteRenderDescription(texture, size,
        CMatrix3x2::CreateTranslation({ -pivot.x, -pivot.y })* transform.ToMatrix())
{
}

CSpriteRenderDescription::CSpriteRenderDescription(
    std::shared_ptr<const CTexture> texture,
    const Creature::Math::Vec2& size,
    const CMatrix3x2& matrix)
    : m_texture(texture)
    , m_size(size)
    , m_matrix(matrix)
{
}

CSpriteRenderDescription& CSpriteRenderDescription::SetPivot(const Vec2& pivot)
{
    m_pivot = pivot;
    return *this;
}

CSpriteRenderDescription& CSpriteRenderDescription::SetOpacity(float fOpacity)
{
    m_fOpacity = fOpacity;
    return *this;
}

CSpriteRenderDescription& CSpriteRenderDescription::SetUv(const Vec2& uvMin, const Vec2& uvMax)
{
    m_uvMin = uvMin;
    m_uvMax = uvMax;
    return *this;
}

const Vec2& CSpriteRenderDescription::GetMinUV() const
{
    return m_uvMin;
}

const Vec2& CSpriteRenderDescription::GetMaxUV() const
{
    return m_uvMax;
}

const Vec2& CSpriteRenderDescription::GetSize() const
{
    return m_size;
}

const std::shared_ptr<const CTexture>& CSpriteRenderDescription::GetTexture() const
{
    return m_texture;
}

const CMatrix3x2& CSpriteRenderDescription::GetMatrix() const
{
    return m_matrix;
}

float CSpriteRenderDescription::GetOpacity() const
{
    return m_fOpacity;
}
