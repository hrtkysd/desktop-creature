#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationPlayer.h"
#include "AnimationPanel.h"
#include "ImGuiWindowScope.h"

#include "imgui.h"
#include "imgui_stdlib.h"

using namespace Creature::Animation;

bool CAnimationPanel::Draw(
    const CAnimation& animation,
    CAnimationPlayer& animationPlayer,
    CAnimationEditor& animationEditor)
{
    CImGuiWindowScope window("Animation");

    bool bChanged = false;

    // Animation Section
    {
        ImGui::SeparatorText("Animation");

        if (ImGui::BeginTable("AnimationProperties", 2))
        {
            ImGui::TableSetupColumn(
                "Label",
                ImGuiTableColumnFlags_WidthFixed,
                100.0f);

            ImGui::TableSetupColumn(
                "Value",
                ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Name");

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);
            auto strName = animation.GetName();

            if (ImGui::InputText(
                "##AnimationName",
                &strName))
            {
                animationEditor.SetName(animation.GetAnimationId(), strName);
                bChanged = true;
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Duration");

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            auto fDuration = animation.GetDuration();
            if (ImGui::DragFloat(
                "##Duration",
                &fDuration,
                0.01f,
                0.0f))
            {
                animationEditor.SetDuration(animation.GetAnimationId(), fDuration);
                bChanged = true;
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();

        {
            ImGui::SeparatorText("Current Time");

            auto fCurrentTime = animationPlayer.GetCurrentAnimationTime();
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::SliderFloat(
                "##CurrentTime",
                &fCurrentTime,
                0.0f,
                animation.GetDuration(),
                "%.2f"))
            {
                animationPlayer.SetCurrentAnimationTime(fCurrentTime);
                bChanged = true;
            }
        }
    }
    return bChanged;
}
