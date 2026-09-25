#include "pch.h"
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

void CMenuBar::Draw(CEditorContext& context)
{
    HandleShortcutKey(context);

    if (CImGuiMainMenuBarScope menuBar{})
    {
        if (CImGuiMenuScope fileMenu{ "File" })
        {
            if (ImGui::MenuItem("Save As..."))
            {
                m_labController.SaveAs();
            }
            else if (ImGui::MenuItem("Load From..."))
            {
                if (m_labController.LoadFrom())
                {
                    context.ClearHistory();
                }
            }
        }

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
                context.GetPartId() != INVALID_PART_ID))
            {
                m_labController.DeletePart();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, context.CanUndo()))
            {
                m_labController.Undo();
            }

            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, context.CanRedo()))
            {
                m_labController.Redo();
            }
        }
    }
}

void CMenuBar::HandleShortcutKey(CEditorContext& context)
{
    const auto& io = ImGui::GetIO();

    if (io.WantTextInput || !io.KeyCtrl) return;

    if (!io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_Z, false) && context.CanUndo())
    {
        m_labController.Undo();
    }
    else if (ImGui::IsKeyPressed(ImGuiKey_Y, false) ||
        (io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_Z, false) && context.CanRedo()))
    {
        m_labController.Redo();
    }
}
