#include "pch.h"
#include "CreatureEditor.h"
#include "DocumentCommand.h"
#include "DocumentController.h"
#include "EditorCommand.h"
#include "EditorController.h"
#include "EditorContext.h"
#include "LabController.h"

CLabController::CLabController(
    CEditorContext& editorContext,
    CDocumentController& documentController,
    CEditorController& editorController,
    CCreatureEditor& creatureEditor)
    : m_editorContext(editorContext)
    , m_documentController(documentController)
    , m_editorController(editorController)
    , m_creatureEditor(creatureEditor)
{
}

void CLabController::SaveAs()
{
    m_documentController.Execute(DocumentCommand::SaveAs);
}

void CLabController::LoadFrom()
{
    m_documentController.Execute(DocumentCommand::LoadFrom);
}

void CLabController::PlayAnimation()
{
    m_editorController.Execute(EditorCommand::PlayAnimation);
}

void CLabController::PauseAnimation()
{
    m_editorController.Execute(EditorCommand::PauseAnimation);
}

void CLabController::StopAnimation()
{
    m_editorController.Execute(EditorCommand::StopAnimation);
}

void CLabController::SetEditMode(EditMode mode)
{
    m_editorContext.SetEditMode(mode);
}

EditMode CLabController::GetEditMode() const noexcept
{
    return m_editorContext.GetEditMode();
}

CCreatureEditor& CLabController::CreatureEditor()
{
    return m_creatureEditor;
}
