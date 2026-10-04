#include "pch.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"

using namespace Creature;
using namespace Creature::Editor;

CCreatureEditor::CCreatureEditor(CCreature& creature)
    : m_creature(creature)
{
}

std::optional<CMotionEditor> CCreatureEditor::MotionEditor(MotionId id)
{
    if (auto motion = m_creature.FindMutableMotionById(id))
    {
        return CMotionEditor(*motion);
    }
    return {};
}

const CMotion* CCreatureEditor::FindMotionById(MotionId id) const
{
    return m_creature.FindMotionById(id);
}

MotionId CCreatureEditor::AddNewMotion(const std::string& strName)
{
    return m_creature.AddNewMotion(strName);
}

MotionId CCreatureEditor::AddMotion(CMotion&& motion)
{
    return m_creature.AddMotion(std::move(motion));
}

MotionId CCreatureEditor::AddMotionWithId(MotionId id, const std::string& strName)
{
    return m_creature.AddMotionWithId(id, strName);
}

bool CCreatureEditor::RemoveMotion(MotionId id)
{
    return m_creature.RemoveMotion(id);
}

const CCreature& CCreatureEditor::GetCreature() const
{
    return m_creature;
}

void CCreatureEditor::SetName(const std::string& strName)
{
    m_creature.SetName(strName);
}

void CCreatureEditor::SwapCreature(CCreature&& creature)
{
    m_creature = std::move(creature);
}
