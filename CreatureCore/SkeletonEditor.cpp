#include "pch.h"
#include "Part.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"
#include "Transform2D.h"
#include "Vec2.h"

using namespace Creature;
using namespace Creature::Editor;
using namespace Creature::Math;

CSkeletonEditor::CSkeletonEditor(
    CSkeleton& skeleton,
    ICreatureEditContext& context)
    : m_skeleton(skeleton)
    , m_context(context)
{
}

bool CSkeletonEditor::SetPartTransform(
    PartId id,
    const CTransform2D& transform)
{
    return m_skeleton.SetPartTransform(id, transform);
}

bool CSkeletonEditor::SetPartPivotAndTransform(
    PartId id,
    const Vec2& pivot,
    const CTransform2D& transform)
{
    return m_skeleton.SetPartPivotAndTransform(id, pivot, transform);
}

bool CSkeletonEditor::SetPartRotation(
    PartId id,
    float fRotation)
{
    return m_skeleton.SetPartRotation(id, fRotation);
}

bool CSkeletonEditor::SetPartPosition(
    PartId id,
    const Vec2& position)
{
    return m_skeleton.SetPartPosition(id, position);
}

bool CSkeletonEditor::SetPartScale(
    PartId id,
    const Vec2& scale)
{
    return m_skeleton.SetPartScale(id, scale);
}

bool CSkeletonEditor::SetPartPivot(
    PartId id,
    const Vec2& pivot)
{
    return m_skeleton.SetPartPivot(id, pivot);
}

PartId CSkeletonEditor::AddPart(
    const std::string_view name,
    PartId parentId)
{
    return m_skeleton.AddPart(name, parentId);
}

PartId CSkeletonEditor::AddPart(Part&& part)
{
    return m_skeleton.AddPart(std::move(part));
}

bool CSkeletonEditor::AddPartWithId(Part&& part)
{
    return m_skeleton.AddPartWithId(std::move(part));
}

bool CSkeletonEditor::RemovePart(PartId id)
{
    return m_context.RemovePart(id);
}

const CSkeleton& CSkeletonEditor::GetSkeleton() const
{
    return m_skeleton;
}

std::unordered_set<PartId> CSkeletonEditor::RemovePartCore(PartId id)
{
    return m_skeleton.RemovePart(id);
}
