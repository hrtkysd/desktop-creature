#include "pch.h"
#include "DocumentContext.h"
#include "EditorContext.h"
#include "ImGuiMainMenuBarScope.h"
#include "ImGuiMenuScope.h"
#include "LabController.h"
#include "MenuBar.h"

#include "imgui.h"

using namespace Creature;

CMenuBar::CMenuBar(CLabController& labController)
    : m_labController(labController)
{
}

void CMenuBar::Draw(
    const CDocumentContext& documentContext,
    CEditorContext& editorContext)
{
    HandleShortcutKey(editorContext);

    if (CImGuiMainMenuBarScope menuBar{})
    {
        if (CImGuiMenuScope fileMenu{ "File" })
        {
            if (ImGui::MenuItem("New Creature"))
            {
                m_labController.CreateNew();
            }
            else if (ImGui::MenuItem("Save As..."))
            {
                m_labController.SaveCreatureAs();
            }
            else if (ImGui::MenuItem("Load From..."))
            {
                m_labController.LoadCreatureFrom();
            }
        }
        const bool hasDocument = documentContext.HasDocument();
        ImGui::BeginDisabled(!hasDocument);

        if (CImGuiMenuScope animationMenu{ "Animation" })
        {
            if (ImGui::MenuItem("Play"))
            {
                m_labController.PlayAnimation();
            }

            if (ImGui::MenuItem("Pause"))
            {
                m_labController.PauseAnimation();
            }

            if (ImGui::MenuItem("Stop"))
            {
                m_labController.StopAnimation();
            }
        }

        if (CImGuiMenuScope editMenu{ "Edit" })
        {
            if (ImGui::MenuItem(
                "Select",
                nullptr,
                m_labController.GetEditMode() == EditMode::Select))
            {
                m_labController.SetEditMode(EditMode::Select);
            }

            if (ImGui::MenuItem(
                "Move",
                nullptr,
                m_labController.GetEditMode() == EditMode::Move))
            {
                m_labController.SetEditMode(EditMode::Move);
            }

            if (ImGui::MenuItem(
                "Scale",
                nullptr,
                m_labController.GetEditMode() == EditMode::Scale))
            {
                m_labController.SetEditMode(EditMode::Scale);
            }

            if (ImGui::MenuItem(
                "Rotate",
                nullptr,
                m_labController.GetEditMode() == EditMode::Rotate))
            {
                m_labController.SetEditMode(EditMode::Rotate);
            }

            if (ImGui::MenuItem(
                "Pivot",
                nullptr,
                m_labController.GetEditMode() == EditMode::Pivot))
            {
                m_labController.SetEditMode(EditMode::Pivot);
            }

            if (ImGui::MenuItem(
                "Delete Part",
                nullptr,
                false,
                editorContext.GetPartId() != INVALID_PART_ID))
            {
                m_labController.DeletePart();
            }

            ImGui::Separator();

            if (ImGui::MenuItem(
                "Move Foward",
                nullptr,
                false,
                m_labController.CanMoveForward()))
            {
                m_labController.MoveForward();
            }
            if (ImGui::MenuItem(
                "Move Backward",
                nullptr,
                false,
                m_labController.CanMoveBackward()))
            {
                m_labController.MoveBackward();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, editorContext.CanUndo()))
            {
                m_labController.Undo();
            }

            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, editorContext.CanRedo()))
            {
                m_labController.Redo();
            }
        }
        ImGui::EndDisabled();
    }
}

void CMenuBar::HandleShortcutKey(CEditorContext& context)
{
    const auto inputFlgs = ImGuiInputFlags_RouteGlobal;

    if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Z, inputFlgs) &&
        context.CanUndo())
    {
        m_labController.Undo();
    }

    if ((ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Y, inputFlgs) ||
        ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Z, inputFlgs)) &&
        context.CanRedo())
    {
        m_labController.Redo();
    }
}
