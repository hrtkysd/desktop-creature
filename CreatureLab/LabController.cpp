#include "pch.h"
#include "Appearance.h"
#include "ApplicationInformation.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "DocumentController.h"
#include "EditorCommand.h"
#include "EditorContext.h"
#include "EditorController.h"
#include "LabController.h"
#include "Motion.h"
#include "PartId.h"
#include "Window.h"

using namespace Creature;
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

void CLabController::OnRevisionChanged(Revision)
{
    if (m_appWindow.Handle() == nullptr) return;
    UpdateWindowTitle();
}

void CLabController::CreateNew()
{
    //TODO: check dirty status, display confirm message.
    m_creatureEditor.SwapCreature({});
    m_creatureEditor.SetName(std::string
        {
            Information::Creature::NewCreatureDefaultName
        });

    m_editorContext.ResetHistory();
    m_editorContext.SelectCreature();

    m_documentController.CreateNewDocument();

    m_revision = m_editorContext.GetRevision();
    UpdateWindowTitle();
}

bool CLabController::SaveCreatureAs()
{
    const auto& creature = m_creatureEditor.GetCreature();
    if (!m_documentController.SaveCreature(creature)) return false;

    m_revision = m_editorContext.GetRevision();

    UpdateWindowTitle();

    return true;
}

bool CLabController::LoadCreatureFrom()
{
    auto loadCreature = m_documentController.LoadCreature();
    if (!loadCreature) return false;

    m_creatureEditor.SwapCreature(std::move(*loadCreature));

    m_editorContext.ResetHistory();
    m_revision = m_editorContext.GetRevision();

    UpdateWindowTitle();

    return true;
}

std::optional<std::filesystem::path> CLabController::LoadAppearance()
{
    return m_documentController.LoadAppearance();
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

bool CLabController::CanMoveForward() const
{
    const auto& creature = m_creatureEditor.GetCreature();
    const auto motion = creature.FindMotionById(m_editorContext.GetMotionId());
    if (!motion) return false;

    const auto partId = m_editorContext.GetPartId();
    if (partId == INVALID_PART_ID) return false;

    const auto& appearance = motion->GetAppearance();
    return appearance.CanMoveForward(partId);
}

bool CLabController::CanMoveBackward() const
{
    const auto& creature = m_creatureEditor.GetCreature();
    const auto motion = creature.FindMotionById(m_editorContext.GetMotionId());
    if (!motion) return false;

    const auto partId = m_editorContext.GetPartId();
    if (partId == INVALID_PART_ID) return false;

    const auto& appearance = motion->GetAppearance();
    return appearance.CanMoveBackward(partId);
}

bool CLabController::MoveForward()
{
    return m_editorController.Execute(EditorCommand::ToForward);
}

bool CLabController::MoveBackward()
{
    return m_editorController.Execute(EditorCommand::ToBackward);
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

    std::string title{ Information::Product::ProductName };

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
