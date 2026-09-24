#include "pch.h"
#include "CreatureEditor.h"
#include "DocumentCommand.h"
#include "DocumentController.h"
#include "EditorCommand.h"
#include "EditorContext.h"
#include "EditorController.h"
#include "LabController.h"

using namespace Creature::Editor;

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

bool CLabController::SaveAs()
{
    return m_documentController.Execute(DocumentCommand::SaveAs);
}

bool CLabController::LoadFrom()
{
    return m_documentController.Execute(DocumentCommand::LoadFrom);
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

bool CLabController::DeletePart()
{
    return m_editorController.Execute(EditorCommand::DeletePart);
}
