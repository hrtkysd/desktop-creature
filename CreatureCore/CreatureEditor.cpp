#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "SkeletonEditor.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;
using namespace Creature::Math;

CCreatureEditor::CCreatureEditor(CCreature& creature)
    : m_creature(creature)
{
}

const CCreature& CCreatureEditor::GetCreature() const
{
    return m_creature;
}

CAnimationEditor CCreatureEditor::GetAnimationEditor()
{
    return CAnimationEditor{ m_creature.MutableAnimations() };
}

CSkeletonEditor CCreatureEditor::GetSkeletonEditor()
{
    return CSkeletonEditor{ m_creature.MutableSkeleton(), *this };
}

CAppearanceEditor CCreatureEditor::GetAppearanceEditor()
{
    return CAppearanceEditor{ m_creature.MutableAppearance() };
}

void CCreatureEditor::SetName(const std::string& strName)
{
    m_creature.SetName(strName);
}

bool CCreatureEditor::RemovePart(PartId id)
{
    const auto parts = GetSkeletonEditor().RemovePartCore(id);
    if (parts.empty()) return false;
    for (const auto part : parts)
    {
        GetAnimationEditor().RemovePart(part);
        GetAppearanceEditor().RemovePart(part);
    }
    return true;
}

const CAnimation* CCreatureEditor::FindAnimationById(AnimationId id) const
{
    return m_creature.FindAnimationById(id);
}

AnimationId CCreatureEditor::AddNewAnimation(const std::string& strName)
{
    return m_creature.AddNewAnimation(strName);
}

AnimationId CCreatureEditor::AddAnimationWithId(AnimationId id, const std::string& strName)
{
    return m_creature.AddAnimationWithId(id, strName);
}

AnimationId CCreatureEditor::AddAnimation(CAnimation&& animation)
{
    return m_creature.AddAnimation(std::move(animation));
}

bool CCreatureEditor::RemoveAnimation(AnimationId id)
{
    return m_creature.RemoveAnimation(id);
}

void CCreatureEditor::SwapCreature(CCreature&& creature)
{
    m_creature = std::move(creature);
}
