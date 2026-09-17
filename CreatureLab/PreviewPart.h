#pragma once

#include "Part.h"
#include "TransformRect.h"
#include "Vec2.h"

class CRenderPartItem;

class CPreviewPart
{
public:
    explicit CPreviewPart(
        Creature::PartId partId,
        const Creature::Math::CTransformRect& rect,
        const Creature::Math::Vec2& size);
public:
    Creature::PartId GetPartId() const noexcept;
    const Creature::Math::CTransformRect& GetRect() const noexcept;
    const Creature::Math::Vec2& GetSize() const noexcept;

private:
    Creature::PartId m_partId;
    Creature::Math::CTransformRect m_rect;
    Creature::Math::Vec2 m_size;

};
