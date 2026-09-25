#include "pch.h"
#include "SpriteRenderDescription.h"
#include "Texture.h"

using namespace Creature::Math;

CSpriteRenderDescription::CSpriteRenderDescription(
    std::shared_ptr<const CTexture> texture,
    const CTransform2D& transform)
    : m_texture(texture)
    , m_transform(transform)
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

const std::shared_ptr<const CTexture>& CSpriteRenderDescription::GetTexture() const
{
    return m_texture;
}

const CTransform2D& CSpriteRenderDescription::GetTransform() const
{
    return m_transform;
}

const Vec2& CSpriteRenderDescription::GetPivot() const
{
    return m_pivot;
}

float CSpriteRenderDescription::GetOpacity() const
{
    return m_fOpacity;
}
