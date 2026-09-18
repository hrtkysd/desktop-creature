#include "pch.h"
#include "EditorContext.h"
#include "ImGuiMainMenuBarScope.h"
#include "ImGuiMenuScope.h"
#include "LabController.h"
#include "MenuBar.h"

#include "imgui.h"

CMenuBar::CMenuBar(CLabController& labController)
    : m_labController(labController)
{
}

void CMenuBar::Draw(CEditorContext& context)
{
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
                m_labController.LoadFrom();
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
        }
    }
}
