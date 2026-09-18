#include "pch.h"
#include "EditorContext.h"

using namespace Creature;
using namespace Creature::Animation;

void CEditorContext::SelectCreature()
{
    m_eSelectonType = SelectionType::Creature;
    m_partId = INVALID_PART_ID;
    m_animationId = INVALID_ANIMATION_ID;
}

void CEditorContext::SelectPart(PartId partId)
{
    m_eSelectonType = SelectionType::Part;
    m_partId = partId;
    m_animationId = INVALID_ANIMATION_ID;
}

void CEditorContext::SelectAnimation(Creature::Animation::AnimationId animationId)
{
    m_eSelectonType = SelectionType::Animation;
    m_partId = INVALID_PART_ID;
    m_animationId = animationId;
}

SelectionType CEditorContext::GetSelectionType() const noexcept
{
    return m_eSelectonType;
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
