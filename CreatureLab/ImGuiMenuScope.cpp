#include "pch.h"
#include "ImGuiMenuScope.h"

#include "imgui.h"

CImGuiMenuScope::CImGuiMenuScope(const char* name)
    : m_bOpen(ImGui::BeginMenu(name))
{
}

CImGuiMenuScope::~CImGuiMenuScope()
{
    if (m_bOpen) ImGui::EndMenu();
}

CImGuiMenuScope::operator bool() const
{
    return m_bOpen;
}
