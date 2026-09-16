#include "pch.h"
#include "ImGuiMainMenuBarScope.h"
#include "ImGuiMenuScope.h"
#include "LabController.h"
#include "MenuBar.h"

#include "imgui.h"

CMenuBar::CMenuBar(CLabController& labController)
    : m_labController(labController)
{
}

void CMenuBar::Render(CEditorContext& context)
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
    }
}
