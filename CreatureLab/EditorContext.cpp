#include "pch.h"
#include "EditorContext.h"
#include "UndoScope.h"

using namespace Creature;
using namespace Creature::Animation;

CEditorContext::CEditorContext(CUndoBuffer& undoBuffer)
    : m_undoBuffer(undoBuffer)
{
}

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

CUndoScope CEditorContext::CreateUndoScope() noexcept
{
    return m_undoBuffer.CreateScope();
}

void CEditorContext::ClearHistory()
{
    m_undoBuffer.Clear();
}

void CEditorContext::Undo()
{
    m_undoBuffer.Undo();
}

void CEditorContext::Redo()
{
    m_undoBuffer.Redo();
}

bool CEditorContext::CanUndo() const noexcept
{
    return m_undoBuffer.CanUndo();
}

bool CEditorContext::CanRedo() const noexcept
{
    return m_undoBuffer.CanRedo();
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
