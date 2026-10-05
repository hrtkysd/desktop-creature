#include "pch.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreatureTreePanel.h"
#include "EditorContext.h"
#include "ImGuiWindowScope.h"
#include "LabController.h"
#include "Motion.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"
#include "UndoScope.h"

#include "imgui.h"
#include "imgui_stdlib.h"

using namespace Creature;
using namespace Creature::Editor;

CCreatureTreePanel::CCreatureTreePanel(
    CCreatureEditor& editor,
    CLabController& labController,
    CEditorContext& context)
    : m_editor(editor)
    , m_labController(labController)
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

    if (ImGui::TreeNode("Motions"))
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("New Motion"))
            {
                m_nodeEdit.BeginCreateNewMotion("New Motion");
            }

            ImGui::EndPopup();
        }

        if (m_nodeEdit.IsEditing(NodeEditType::CreateMotion))
        {
            if (ImGui::InputText(
                "##NewMotion",
                &m_nodeEdit.GetText(),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                auto undoScope = m_editorContext.CreateUndoScope();
                const auto newId = m_editor.AddNewMotion(m_nodeEdit.GetText());
                m_editorContext.SelectMotion(newId);
                m_nodeEdit.EndEdit();
            }
        }

        for (const auto& motion : creature.GetMotions())
        {
            const auto motionId = motion.GetMotionId();

            ImGui::PushID(static_cast<int>(motionId));

            auto imGuiFlags =
                ImGuiTreeNodeFlags_OpenOnArrow |
                ImGuiTreeNodeFlags_SpanAvailWidth;

            if (m_editorContext.GetMotionId() == motionId &&
                m_editorContext.GetSelectionType() == SelectionType::Motion)
            {
                imGuiFlags |= ImGuiTreeNodeFlags_Selected;
            }

            const auto isMotionOpen = ImGui::TreeNodeEx("##Motion", imGuiFlags);

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                m_editorContext.SelectMotion(motionId);
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Rename"))
                {
                    m_nodeEdit.BeginRenameMotion(motionId, motion.GetName());
                }

                ImGui::EndPopup();
            }

            ImGui::SameLine();

            if (m_nodeEdit.IsEditing(NodeEditType::RenameMotion) &&
                m_nodeEdit.GetMotionId() == motionId)
            {
                ImGui::SetNextItemWidth(150.0f);

                if (ImGui::InputText(
                    "##MotionName",
                    &m_nodeEdit.GetText(),
                    ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (auto motionEditor = m_editor.MotionEditor(motionId))
                    {
                        auto scope = m_editorContext.CreateUndoScope();
                        motionEditor->SetName(m_nodeEdit.GetText());
                    }

                    m_nodeEdit.EndEdit();
                }
            }
            else
            {
                ImGui::TextUnformatted(motion.GetName().c_str());
            }

            if (isMotionOpen)
            {
                const auto& skeleton = motion.GetSkeleton();

                if (ImGui::TreeNode("Parts"))
                {
                    if (ImGui::BeginPopupContextItem())
                    {
                        if (ImGui::MenuItem("New Part"))
                        {
                            m_nodeEdit.BeginCreatePart(motionId, INVALID_PART_ID, "New Part");
                        }

                        ImGui::EndPopup();
                    }

                    if (m_nodeEdit.IsEditing(NodeEditType::CreatePart) &&
                        m_nodeEdit.GetMotionId() == motionId &&
                        m_nodeEdit.GetParentPartId() == INVALID_PART_ID)
                    {
                        if (ImGui::InputText(
                            "##NewPart",
                            &m_nodeEdit.GetText(),
                            ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            if (auto motionEditor = m_editor.MotionEditor(motionId))
                            {
                                auto undoScope = m_editorContext.CreateUndoScope();

                                const auto partId =
                                    motionEditor->SkeletonEditor().AddPart(m_nodeEdit.GetText());

                                if (partId != INVALID_PART_ID)
                                {
                                    m_editorContext.SelectMotion(motionId);
                                    m_editorContext.SelectPart(partId);
                                }
                            }

                            m_nodeEdit.EndEdit();
                        }
                    }

                    for (const auto partId : skeleton.GetRootPartIds())
                    {
                        DrawPart(motionId, skeleton, partId);
                    }

                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    ImGui::TreePop();
    ImGui::PopID();
}

void CCreatureTreePanel::DrawPart(
    MotionId motionId,
    const CSkeleton& skeleton,
    PartId partId)
{
    const auto part = skeleton.FindPartById(partId);
    if (!part) return;

    const auto isSelected =
        m_editorContext.GetSelectionType() == SelectionType::Part &&
        m_editorContext.GetMotionId() == motionId &&
        m_editorContext.GetPartId() == partId;

    auto imGuiFlags =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    if (!skeleton.HasChildren(partId))
    {
        imGuiFlags |=
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    if (isSelected)
    {
        imGuiFlags |= ImGuiTreeNodeFlags_Selected;
    }

    const auto isOpen = ImGui::TreeNodeEx(
        reinterpret_cast<void*>(static_cast<uintptr_t>(partId)),
        imGuiFlags,
        "%s",
        part->strName.c_str());

    if (ImGui::IsItemClicked())
    {
        m_editorContext.SelectMotion(motionId);
        m_editorContext.SelectPart(partId);
    }

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("New Child"))
        {
            m_nodeEdit.BeginCreatePart(motionId, partId, "New Part");
        }

        if (ImGui::MenuItem("Set Texture"))
        {
            m_editorContext.SelectMotion(motionId);
            m_editorContext.SelectPart(partId);

            if (auto motionEditor = m_editor.MotionEditor(motionId))
            {
                const auto texturePath = m_labController.LoadAppearance();
                if (texturePath && !texturePath->empty())
                {
                    motionEditor->AppearanceEditor().AddPart(partId, *texturePath);
                }
            }
        }

        ImGui::EndPopup();
    }

    if (m_nodeEdit.IsEditing(NodeEditType::CreatePart) &&
        m_nodeEdit.GetMotionId() == motionId &&
        m_nodeEdit.GetParentPartId() == partId)
    {
        ImGui::Indent();

        if (ImGui::InputText(
            "##NewPart",
            &m_nodeEdit.GetText(),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (auto motionEditor = m_editor.MotionEditor(motionId))
            {
                auto undoScope = m_editorContext.CreateUndoScope();

                const auto newPartId =
                    motionEditor->SkeletonEditor().AddPart(m_nodeEdit.GetText(), partId);

                if (newPartId != INVALID_PART_ID)
                {
                    m_editorContext.SelectMotion(motionId);
                    m_editorContext.SelectPart(newPartId);
                }
            }

            m_nodeEdit.EndEdit();
        }

        ImGui::Unindent();
    }

    if (isOpen && !(imGuiFlags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
    {
        for (const auto childId : skeleton.GetChildPartIds(partId))
        {
            DrawPart(motionId, skeleton, childId);
        }

        ImGui::TreePop();
    }
}
