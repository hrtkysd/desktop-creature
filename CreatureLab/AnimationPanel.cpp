#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationPanel.h"
#include "AnimationPlayer.h"
#include "AnimationProperty.h"
#include "ContinuousEditUndoScope.h"
#include "EditorContext.h"
#include "ImGuiWindowScope.h"
#include "Skeleton.h"

#include "imgui.h"
#include "imgui_stdlib.h"

using namespace Creature;
using namespace Creature::Editor;
using namespace Creature::Animation;

namespace
{
    const char* GetAnimationPropertyName(AnimationProperty eProperty)
    {
        switch (eProperty)
        {
        case AnimationProperty::PositionX: return "PositionX";
        case AnimationProperty::PositionY: return "PositionY";
        case AnimationProperty::Rotation: return "Rotation";
        case AnimationProperty::ScaleX: return "ScaleX";
        case AnimationProperty::ScaleY: return "ScaleY";
        }
        return "";
    }

    const char* GetInterpolationName(Interpolation eInterpolation)
    {
        switch (eInterpolation)
        {
        case Interpolation::Linear: return "Linear";
        case Interpolation::SmoothStep: return "SmoothStep";
        case Interpolation::Step: return "Step";
        }
        return "";
    }
}

CAnimationPanel::CAnimationPanel(
    CAnimationPlayer& animationPlayer,
    CAnimationEditor& editor,
    CEditorContext& editorContext)
    : m_animationPlayer(animationPlayer)
    , m_editor(editor)
    , m_editorContext(editorContext)
{
}
bool CAnimationPanel::Draw(
    const Creature::Animation::CAnimation& animation,
    const Creature::CSkeleton& skeleton)
{
    if (m_animationId != animation.GetAnimationId())
    {
        m_animationId = animation.GetAnimationId();
        m_selectedTrackKey.reset();
    }

    CImGuiWindowScope window("Animation");

    bool bChanged = false;

    bChanged |= DrawAnimationProperties(animation);

    DrawCurrentTime(animation);

    bChanged |= DrawTracks(animation, skeleton);

    return bChanged;
}

bool CAnimationPanel::DrawAnimationProperties(const CAnimation& animation)
{
    bool bChanged = false;

    ImGui::SeparatorText("Animation");

    if (!ImGui::BeginTable(
        "AnimationProperties",
        2))
    {
        return false;
    }

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

    {
        auto strName = animation.GetName();
        const auto edited = ImGui::InputText(
            "##AnimationName",
            &strName);

        CContinuousEditUndoScope scope(m_editorContext, m_undoScope, edited);

        if (edited)
        {
            if (m_editor.SetName(animation.GetAnimationId(), strName))
            {
                bChanged = true;
            }
        }
    }

    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted("Duration");

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);

    {
        auto fDuration = animation.GetDuration();
        const auto edited = ImGui::DragFloat(
            "##Duration",
            &fDuration,
            0.01f,
            0.0f);

        CContinuousEditUndoScope scope(m_editorContext, m_undoScope, edited);
        if (edited)
        {
            m_editor.SetDuration(
                animation.GetAnimationId(),
                fDuration);

            bChanged = true;
        }
    }
    ImGui::EndTable();

    return bChanged;
}

void CAnimationPanel::DrawCurrentTime(const CAnimation& animation)
{
    ImGui::Spacing();
    ImGui::SeparatorText("Current Time");

    auto fCurrentTime = m_animationPlayer.GetCurrentAnimationTime();

    ImGui::SetNextItemWidth(-FLT_MIN);

    const auto edited = ImGui::SliderFloat(
        "##CurrentTime",
        &fCurrentTime,
        0.0f,
        animation.GetDuration(),
        "%.2f");

    CContinuousEditUndoScope scope(m_editorContext, m_undoScope, edited);
    if (edited)
    {
        m_animationPlayer.SetCurrentAnimationTime(fCurrentTime);
    }
}

bool CAnimationPanel::DrawTracks(
    const CAnimation& animation,
    const Creature::CSkeleton& skeleton)
{
    bool bChanged = false;

    ImGui::Spacing();
    ImGui::SeparatorText("Tracks");

    for (const auto& track : animation.GetAnimationTracks())
    {
        const auto& key = track.GetKey();

        const bool isSelected =
            m_selectedTrackKey.has_value() &&
            *m_selectedTrackKey == key;

        ImGui::PushID(
            static_cast<int>(key.GetPartId()));

        ImGui::PushID(
            static_cast<int>(key.GetProperty()));

        std::string strPartName;
        if (const auto part = skeleton.FindPartById(key.GetPartId()))
        {
            strPartName = part->strName;
        }
        const auto strLabel =
            strPartName +
            " / " +
            GetAnimationPropertyName(key.GetProperty());
        if (ImGui::Selectable(
            strLabel.c_str(),
            isSelected))
        {
            m_selectedTrackKey = key;
        }

        ImGui::PopID();
        ImGui::PopID();
    }

    bChanged |= DrawAddTrack(animation);

    if (!m_selectedTrackKey) return bChanged;

    if (const auto track = animation.FindAnimationTrack(*m_selectedTrackKey))
    {
        bChanged |= DrawTrack(animation, *track);
    }
    else
    {
        m_selectedTrackKey.reset();
    }

    return bChanged;
}

bool CAnimationPanel::DrawAddTrack(const CAnimation& animation)
{
    const auto partId = m_editorContext.GetPartId();

    if (partId == INVALID_PART_ID)
    {
        ImGui::TextDisabled("Select a part first.");
        return false;
    }

    ImGui::Spacing();

    ImGui::Text("Part: %u", partId);

    ImGui::SameLine();

    ImGui::SetNextItemWidth(120.0f);

    if (ImGui::BeginCombo(
        "##AnimationProperty",
        GetAnimationPropertyName(m_eNewTrackProperty)))
    {
        constexpr AnimationProperty eProperties[]
        {
            AnimationProperty::PositionX,
            AnimationProperty::PositionY,
            AnimationProperty::Rotation,
            AnimationProperty::ScaleX,
            AnimationProperty::ScaleY
        };

        for (const auto eProperty : eProperties)
        {
            const auto isSelected = eProperty == m_eNewTrackProperty;

            if (ImGui::Selectable(
                GetAnimationPropertyName(eProperty),
                isSelected))
            {
                m_eNewTrackProperty = eProperty;
            }
        }

        ImGui::EndCombo();
    }

    ImGui::SameLine();

    const CAnimationTrackKey key(partId, m_eNewTrackProperty);

    const bool bExists = animation.FindAnimationTrack(key) != nullptr;

    ImGui::BeginDisabled(bExists);

    bool bChanged = false;
    if (ImGui::Button("Add Track"))
    {
        auto scope = m_editorContext.CreateUndoScope();
        if (m_editor.AddTrack(animation.GetAnimationId(), CAnimationTrack{ key }))
        {
            m_selectedTrackKey = key;
            bChanged = true;
        }
        else
        {
            scope.Cancel();
        }
    }

    ImGui::EndDisabled();

    return bChanged;
}

bool CAnimationPanel::DrawTrack(const CAnimation& animation, const CAnimationTrack& track)
{
    bool bChanged = false;

    const auto& trackKey = track.GetKey();

    ImGui::Spacing();
    ImGui::SeparatorText("Keyframes");

    for (const auto& keyFrame : track.GetKeyFrames())
    {
        ImGui::PushID(&keyFrame);

        ImGui::Text("%.2f", keyFrame.fTime);

        ImGui::SameLine();

        auto fValue = keyFrame.fValue;

        ImGui::SetNextItemWidth(100.0f);
        {
            const auto edited = ImGui::DragFloat(
                "##Value",
                &fValue,
                0.01f);

            CContinuousEditUndoScope scope(m_editorContext, m_undoScope, edited);

            if (edited)
            {
                auto edited = keyFrame;
                edited.fValue = fValue;
                m_editor.AddOrUpdateKeyFrame(animation.GetAnimationId(), trackKey, edited);

                bChanged = true;
            }
        }
        ImGui::SameLine();

        auto interpolation = keyFrame.eInterpolationToNext;

        ImGui::SetNextItemWidth(120.0f);

        {
            if (ImGui::BeginCombo(
                "##Interpolation",
                GetInterpolationName(interpolation)))
            {
                constexpr Interpolation values[]
                {
                    Interpolation::Linear,
                    Interpolation::SmoothStep,
                    Interpolation::Step
                };

                for (const auto value : values)
                {
                    auto scope = m_editorContext.CreateUndoScope();
                    if (ImGui::Selectable(
                        GetInterpolationName(value),
                        interpolation == value))
                    {
                        auto edited = keyFrame;

                        edited.eInterpolationToNext = value;

                        m_editor.AddOrUpdateKeyFrame(animation.GetAnimationId(), trackKey, edited);

                        bChanged = true;
                    }
                    else
                    {
                        scope.Cancel();
                    }
                }

                ImGui::EndCombo();
            }
        }

        ImGui::PopID();
    }

    ImGui::Spacing();

    if (ImGui::Button("Add Keyframe"))
    {
        auto scope = m_editorContext.CreateUndoScope();

        const auto fTime = m_animationPlayer.GetCurrentAnimationTime();

        FloatKeyFrame keyFrame
        {
            fTime,
            track.Sample(fTime),
            Interpolation::SmoothStep
        };

        if (m_editor.AddOrUpdateKeyFrame(animation.GetAnimationId(), trackKey, keyFrame))
        {
            bChanged = true;
        }
        else
        {
            scope.Cancel();
        }

    }

    ImGui::SameLine();

    if (ImGui::Button("Delete Track"))
    {
        auto scope = m_editorContext.CreateUndoScope();
        if (m_editor.RemoveTrack(animation.GetAnimationId(), trackKey))
        {
            m_selectedTrackKey.reset();
            bChanged = true;
        }
        else
        {
            scope.Cancel();
        }
    }

    return bChanged;
}
