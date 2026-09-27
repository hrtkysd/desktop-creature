#pragma once

#include "Matrix3x2.h"
#include "Vec2.h"

#include <memory>

namespace Creature
{
    namespace Math
    {
        class CTransform2D;
    }
}

class CTexture;

class CSpriteRenderDescription
{
public:
    CSpriteRenderDescription(
        std::shared_ptr<const CTexture> texture,
        const Creature::Math::Vec2& size,
        const Creature::Math::CTransform2D& transform,
        const Creature::Math::Vec2& pivot);
    CSpriteRenderDescription(
        std::shared_ptr<const CTexture> texture,
        const Creature::Math::Vec2& size,
        const Creature::Math::CMatrix3x2& transform);

    CSpriteRenderDescription& SetPivot(const Creature::Math::Vec2& pivot);
    CSpriteRenderDescription& SetOpacity(float fOpacity);
    CSpriteRenderDescription& SetUv(
        const Creature::Math::Vec2& uvMin,
        const Creature::Math::Vec2& uvMax);

    const Creature::Math::Vec2& GetMinUV() const;
    const Creature::Math::Vec2& GetMaxUV() const;

    const Creature::Math::Vec2& GetSize() const;

    const std::shared_ptr<const CTexture>& GetTexture() const;
    const Creature::Math::CMatrix3x2& GetMatrix() const;
    float GetOpacity() const;

private:
    std::shared_ptr <const CTexture> m_texture;

    Creature::Math::CMatrix3x2 m_matrix;
    Creature::Math::Vec2 m_pivot{};
    Creature::Math::Vec2 m_uvMin{ 0.0f, 0.0f };
    Creature::Math::Vec2 m_uvMax{ 1.0f, 1.0f };
    Creature::Math::Vec2 m_size{ 0.0f, 0.0f };
    float m_fOpacity = 1.0f;
};
