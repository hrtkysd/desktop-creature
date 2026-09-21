#include "pch.h"
#include "Animation.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreatureTreePanel.h"
#include "EditorContext.h"
#include "ImGuiWindowScope.h"
#include "PartId.h"
#include "Skeleton.h"

#include "imgui.h"
#include "imgui_stdlib.h"

using namespace Creature;
using namespace Creature::Animation;

CCreatureTreePanel::CCreatureTreePanel(
    CCreatureEditor& editor,
    CEditorContext& context)
    : m_editor(editor)
    , m_editorContext(context)
{
}

void CCreatureTreePanel::Draw()
{
    const auto& creature = m_editor.GetCreature();

    CImGuiWindowScope window("Creature");

    ImGui::PushID("Creature");

    const auto treeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
    const auto isOpen = ImGui::TreeNodeEx("##CreatureTree", treeFlags);

    ImGui::SameLine();
    if (m_nodeEdit.IsEditing(NodeEditType::RenameCreature))
    {
        ImGui::SetNextItemWidth(150.0f);

        if (ImGui::InputText(
            "##CreatureName",
            &m_nodeEdit.GetText(),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_editor.SetName(m_nodeEdit.GetText());
            m_nodeEdit.EndEdit();
        }
    }
    else
    {
        const auto isSelected = m_editorContext.GetSelectionType() == SelectionType::Creature;

        if (ImGui::Selectable(creature.GetName().c_str(), isSelected))
        {
            m_editorContext.SelectCreature();
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Rename"))
            {
                m_nodeEdit.BeginRenameCreature(creature.GetName());
            }

            ImGui::EndPopup();
        }
    }

    if (!isOpen)
    {
        ImGui::PopID();
        return;
    }

    if (ImGui::TreeNode("Animations"))
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("New Animation"))
            {
                m_nodeEdit.BeginCreateNewAnimation("New Animation");
            }

            ImGui::EndPopup();
        }

        if (m_nodeEdit.IsEditing(NodeEditType::CreateAnimation))
        {
            if (ImGui::InputText(
                "##NewAnimation",
                &m_nodeEdit.GetText(),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                const auto newId = m_editor.AddNewAnimation(m_nodeEdit.GetText());
                m_editorContext.SelectAnimation(newId);
                m_nodeEdit.EndEdit();
            }
        }

        for (const auto& animation : creature.GetAnimations())
        {
            ImGui::PushID(animation.GetAnimationId());

            ImGuiTreeNodeFlags flags =
                ImGuiTreeNodeFlags_Leaf |
                ImGuiTreeNodeFlags_NoTreePushOnOpen |
                ImGuiTreeNodeFlags_SpanAvailWidth;

            if (m_editorContext.GetAnimationId() == animation.GetAnimationId())
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            ImGui::TreeNodeEx(animation.GetName().c_str(), flags);

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                m_editorContext.SelectAnimation(animation.GetAnimationId());
            }

            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Parts"))
    {
        const auto& skeleton = creature.GetSkeleton();

        for (const auto& id : skeleton.GetRootPartIds())
        {
            DrawPart(skeleton, id);
        }

        ImGui::TreePop();
    }
    ImGui::TreePop();
    ImGui::PopID();
}

void CCreatureTreePanel::DrawPart(
    const Creature::CSkeleton& skeleton,
    Creature::PartId partId)
{
    const auto part = skeleton.FindPartById(partId);
    if (!part) return;

    const bool isSelected =
        m_editorContext.GetSelectionType() == SelectionType::Part &&
        m_editorContext.GetPartId() == partId;

    auto imguiFlags =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    if (!skeleton.HasChildren(partId))
    {
        imguiFlags |=
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    if (isSelected) imguiFlags |= ImGuiTreeNodeFlags_Selected;

    const auto isOpen =
        ImGui::TreeNodeEx(
            reinterpret_cast<void*>(
                static_cast<uintptr_t>(partId)),
            imguiFlags,
            "%s",
            part->strName.c_str());

    if (ImGui::IsItemClicked())
    {
        m_editorContext.SelectPart(partId);
    }

    if (isOpen && !(imguiFlags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
    {
        for (const auto childId : skeleton.GetChildPartIds(partId))
        {
            DrawPart(skeleton, childId);
        }

        ImGui::TreePop();
    }
}
