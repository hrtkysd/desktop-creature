#pragma once

#include "Part.h"
#include "TransformRect.h"

#include <memory>

class CTexture;

class CRenderPartItem
{
public:
    explicit CRenderPartItem(
        const Creature::PartId partId,
        const Creature::Math::CTransformRect& rcPart,
        const std::shared_ptr<CTexture>& texture);
public:
    Creature::PartId GetPartId() const;
    const Creature::Math::CTransformRect GetRect() const;
    const std::shared_ptr<CTexture>& GetTexture() const;

    bool IsPtInView(const Creature::Math::Vec2& pt) const;
private:
    Creature::PartId m_partId = Creature::INVALID_PART_ID;
    Creature::Math::CTransformRect m_rcPart;
    std::shared_ptr<CTexture> m_texture;
};
