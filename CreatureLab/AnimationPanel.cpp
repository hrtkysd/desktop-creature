#include "pch.h"
#include "Animation.h"
#include "AnimationPanel.h"
#include "ImGuiWindowScope.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace
{
}

bool CAnimationPanel::Draw(Creature::Animation::CAnimation& animation)
{
    CImGuiWindowScope window("Animation");

    bool bChanged = false;

    std::string strAnimation = animation.GetName();
    ImGui::Text("Name");

    if (ImGui::InputText("##AnimationName", &strAnimation))
    {
        animation.SetName(strAnimation);
        bChanged = true;
    }

    return bChanged;
}
