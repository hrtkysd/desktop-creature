#include "pch.h"
#include "EditorContext.h"

using namespace Creature;
using namespace Creature::Animation;

void CEditorContext::SelectCreature()
{
    m_eSelectionType = SelectionType::Creature;
    m_partId = INVALID_PART_ID;
}

void CEditorContext::SelectPart(PartId partId)
{
    m_eSelectionType = SelectionType::Part;
    m_partId = partId;
}

void CEditorContext::SelectAnimation(Creature::Animation::AnimationId animationId)
{
    m_eSelectionType = SelectionType::Animation;
    m_animationId = animationId;
}

SelectionType CEditorContext::GetSelectionType() const noexcept
{
    return m_eSelectionType;
}

EditMode CEditorContext::GetEditMode() const noexcept
{
    return m_eEditMode;
}

void CEditorContext::SetEditMode(EditMode mode)
{
    m_eEditMode = mode;
}

PartId CEditorContext::GetPartId() const noexcept
{
    return m_partId;
}

AnimationId CEditorContext::GetAnimationId() const noexcept
{
    return m_animationId;
}
