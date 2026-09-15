#include "pch.h"
#include "RenderPartItem.h"

using namespace Creature;
using namespace Creature::Math;

CRenderPartItem::CRenderPartItem(
    const PartId partId,
    const CTransformRect& rcPart,
    const std::shared_ptr<CTexture>& texture)
    : m_partId(partId)
    , m_rcPart(rcPart)
    , m_texture(texture) {}

PartId CRenderPartItem::GetPartId() const
{
    return m_partId;
}

const CTransformRect CRenderPartItem::GetRect() const
{
    return m_rcPart;
}

const std::shared_ptr<CTexture>& CRenderPartItem::GetTexture() const
{
    return m_texture;
}

bool CRenderPartItem::IsPtInView(const Vec2& pt) const
{
    return m_rcPart.Contains(pt);
}
