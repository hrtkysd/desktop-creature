#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationPlayer.h"
#include "AnimationPanel.h"
#include "ImGuiWindowScope.h"

#include "imgui.h"
#include "imgui_stdlib.h"

using namespace Creature::Animation;

CAnimationPanel::CAnimationPanel(
    CAnimationPlayer& animationPlayer,
    CAnimationEditor& animationEditor,
    CEditorContext& editorContext)
    : m_animationPlayer(animationPlayer)
    , m_editor(animationEditor)
    , m_editorContext(editorContext)
{
}
bool CAnimationPanel::Draw(
    const Creature::Animation::CAnimation& animation,
    const Creature::CSkeleton& skeleton)
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
                m_editor.SetName(animation.GetAnimationId(), strName);
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
                m_editor.SetDuration(animation.GetAnimationId(), fDuration);
                bChanged = true;
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();

        {
            ImGui::SeparatorText("Current Time");

            auto fCurrentTime = m_animationPlayer.GetCurrentAnimationTime();
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::SliderFloat(
                "##CurrentTime",
                &fCurrentTime,
                0.0f,
                animation.GetDuration(),
                "%.2f"))
            {
                m_animationPlayer.SetCurrentAnimationTime(fCurrentTime);
                bChanged = true;
            }
        }
    }
    return bChanged;
}
