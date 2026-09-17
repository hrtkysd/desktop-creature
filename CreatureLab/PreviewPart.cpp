#include "pch.h"
#include "PreviewPart.h"

using namespace Creature;
using namespace Math;

CPreviewPart::CPreviewPart(
    PartId partId,
    const CTransformRect& rect,
    const Vec2& size)
    : m_partId(partId)
    , m_rect(rect)
    , m_size(size)
{
}

PartId CPreviewPart::GetPartId() const noexcept
{
    return m_partId;
}

const CTransformRect& CPreviewPart::GetRect() const noexcept
{
    return m_rect;
}

const Vec2& CPreviewPart::GetSize() const noexcept
{
    return m_size;
}
