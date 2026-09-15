#include "pch.h"
#include "ImGuiMainMenuBarScope.h"

#include "imgui.h"

CImGuiMainMenuBarScope::CImGuiMainMenuBarScope()
    : m_bOpened(ImGui::BeginMainMenuBar())
{
}

CImGuiMainMenuBarScope::~CImGuiMainMenuBarScope()
{
    if (m_bOpened) ImGui::EndMainMenuBar();
}

CImGuiMainMenuBarScope::operator bool() const
{
    return m_bOpened;
}
