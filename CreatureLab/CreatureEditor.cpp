#include "pch.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Skeleton.h"

using namespace Creature;
using namespace Creature::Math;

CCreatureEditor::CCreatureEditor(CCreature& creature)
    : m_creature(creature)
{
}

bool CCreatureEditor::SetPartTransform(Creature::PartId id, const Creature::Math::CTransform2D& transform)
{
    auto part = m_creature.GetSkeleton().FindPartById(id);
    if (!part) return false;

    part->bindTransform = transform;
    return true;
}

bool CCreatureEditor::SetPartPivotAndTransform(Creature::PartId id, const Vec2& pivot, const CTransform2D& transform)
{
    auto part = m_creature.GetSkeleton().FindPartById(id);
    if (!part) return false;
    part->pivot = pivot;
    part->bindTransform = transform;
    return true;
}

void CCreatureEditor::SetPartRotation(PartId id, float rotation)
{
    auto part = m_creature.GetSkeleton().FindPartById(id);
    if (part == nullptr) return;
    part->bindTransform.SetRotation(rotation);
}

void CCreatureEditor::SetPartPosition(Creature::PartId id, const Creature::Math::Vec2& position)
{
    auto part = m_creature.GetSkeleton().FindPartById(id);
    if (part == nullptr) return;
    part->bindTransform.SetPosition(position);
}

void CCreatureEditor::SetPartScale(PartId id, const Vec2& scale)
{
    auto part = m_creature.GetSkeleton().FindPartById(id);
    if (part == nullptr) return;
    part->bindTransform.SetScale(scale);
}

void CCreatureEditor::SetPartPivot(Creature::PartId id, const Creature::Math::Vec2& pivot)
{
    auto part = m_creature.GetSkeleton().FindPartById(id);
    if (part == nullptr) return;
    part->pivot = pivot;
}

bool CCreatureEditor::DeletePart(Creature::PartId id)
{
    return m_creature.GetSkeleton().RemovePart(id);
}

void CCreatureEditor::SetName(const std::string& strName)
{
    m_creature.SetName(strName);
}
