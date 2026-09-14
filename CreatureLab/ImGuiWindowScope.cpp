#include "pch.h"
#include "imgui.h"

#include "ImGuiWindowScope.h"

CImGuiWindowScope::CImGuiWindowScope(const char* name)
{
    m_bVisible = ImGui::Begin(name);
}

CImGuiWindowScope::~CImGuiWindowScope()
{
    ImGui::End();
}

CImGuiWindowScope::operator bool() const
{
    return m_bVisible;
}
