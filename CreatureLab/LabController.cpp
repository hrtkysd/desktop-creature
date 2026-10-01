#include "pch.h"
#include "ApplicationInformation.h"
#include "CreatureEditor.h"
#include "DocumentController.h"
#include "EditorCommand.h"
#include "EditorContext.h"
#include "EditorController.h"
#include "LabController.h"
#include "Window.h"

using namespace Creature::Editor;

CLabController::CLabController(
    CWindow& appWindow,
    CEditorContext& editorContext,
    CDocumentController& documentController,
    CEditorController& editorController,
    CCreatureEditor& creatureEditor)
    : m_appWindow(appWindow)
    , m_editorContext(editorContext)
    , m_documentController(documentController)
    , m_editorController(editorController)
    , m_creatureEditor(creatureEditor)
{
}

void CLabController::OnRevisionChanged(Revision revision)
{
    if (m_appWindow.Handle() == nullptr) return;
    UpdateWindowTitle();
}

bool CLabController::SaveAs()
{
    const auto& creature = m_creatureEditor.GetCreature();
    if (!m_documentController.Save(creature)) return false;

    m_revision = m_editorContext.GetRevision();

    UpdateWindowTitle();

    return true;
}

bool CLabController::LoadFrom()
{
    auto loadCreature = m_documentController.Load();
    if (!loadCreature) return false;

    m_creatureEditor.SwapCreature(std::move(*loadCreature));

    m_editorContext.ResetHistory();
    m_revision = m_editorContext.GetRevision();

    UpdateWindowTitle();

    return true;
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

void CLabController::Undo()
{
    m_editorController.Execute(EditorCommand::Undo);
}

void CLabController::Redo()
{
    m_editorController.Execute(EditorCommand::Redo);
}

bool CLabController::IsDirty() const noexcept
{
    return m_revision != m_editorContext.GetRevision();
}

void CLabController::UpdateWindowTitle()
{
    const auto& creature = m_creatureEditor.GetCreature();

    std::string title{ Information::ProductName };

    if (!creature.GetName().empty())
    {
        title += " - ";
        title += creature.GetName();
    }

    if (IsDirty())
    {
        title += " *";
    }

    m_appWindow.SetTitle(title);
}
