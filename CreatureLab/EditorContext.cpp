#include "pch.h"
#include "EditorContext.h"
#include "UndoScope.h"

using namespace Creature;

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

void CEditorContext::SelectMotion(Creature::MotionId motionId)
{
    m_eSelectionType = SelectionType::Motion;
    m_motionId = motionId;
}

SelectionType CEditorContext::GetSelectionType() const noexcept
{
    return m_eSelectionType;
}

CUndoScope CEditorContext::CreateUndoScope() noexcept
{
    return m_undoBuffer.CreateScope();
}

void CEditorContext::ResetHistory()
{
    m_undoBuffer.ResetHistory();
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

Revision CEditorContext::GetRevision() const noexcept
{
    return m_undoBuffer.GetCurrentRevision();
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

MotionId CEditorContext::GetMotionId() const noexcept
{
    return m_motionId;
}
