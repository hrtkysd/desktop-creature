#include "pch.h"
#include "Animation.h"
#include "Creature.h"
#include "CreatureEditor.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;
using namespace Creature::Math;

CCreatureEditor::CCreatureEditor(CCreature& creature)
    : m_creature(creature)
    , m_animationEditor(creature)
    , m_skeletonEditor(creature.GetSkeleton(), *this)
    , m_appearanceEditor(creature.GetAppearance())
{
}

const CCreature& CCreatureEditor::GetCreature() const
{
    return m_creature;
}

CAnimationEditor& CCreatureEditor::GetAnimationEditor()
{
    return m_animationEditor;
}

CSkeletonEditor& CCreatureEditor::GetSkeletonEditor()
{
    return m_skeletonEditor;
}

CAppearanceEditor& CCreatureEditor::GetAppearanceEditor()
{
    return m_appearanceEditor;
}

void CCreatureEditor::SetName(const std::string& strName)
{
    m_creature.SetName(strName);
}

CAnimation* CCreatureEditor::FindAnimationById(AnimationId id)
{
    return m_creature.FindAnimationById(id);
}

bool CCreatureEditor::RemovePart(PartId id)
{
    const auto parts = m_skeletonEditor.RemovePartCore(id);
    if (parts.empty()) return false;
    for (const auto part : parts)
    {
        m_animationEditor.RemovePart(part);
        m_appearanceEditor.RemovePart(part);
    }
    return true;
}

const CAnimation* CCreatureEditor::FindAnimationById(AnimationId id) const
{
    return m_creature.FindReadonlyAnimationById(id);
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
