#pragma once

#include "Transform2D.h"
#include "Vec2.h"

#include <memory>

class CTexture;

class CSpriteRenderDescription
{
public:
    CSpriteRenderDescription(
        std::shared_ptr<const CTexture> texture,
        const Creature::Math::CTransform2D& transform);

    CSpriteRenderDescription& SetPivot(const Creature::Math::Vec2& pivot);
    CSpriteRenderDescription& SetOpacity(float fOpacity);
    CSpriteRenderDescription& SetUv(
        const Creature::Math::Vec2& uvMin,
        const Creature::Math::Vec2& uvMax);

    const std::shared_ptr<const CTexture>& GetTexture() const;
    const Creature::Math::CTransform2D& GetTransform() const;
    const Creature::Math::Vec2& GetPivot() const;
    float GetOpacity() const;

private:
    std::shared_ptr <const CTexture> m_texture;

    Creature::Math::CTransform2D m_transform{};
    Creature::Math::Vec2 m_pivot{};
    Creature::Math::Vec2 m_uvMin{ 0.0f, 0.0f };
    Creature::Math::Vec2 m_uvMax{ 1.0f, 1.0f };

    float m_fOpacity = 1.0f;
};
