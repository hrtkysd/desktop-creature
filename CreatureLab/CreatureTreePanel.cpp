#include "pch.h"
#include "AnimationEntry.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreatureTreePanel.h"
#include "EditorContext.h"
#include "ImGuiWindowScope.h"

#include "imgui.h"

using namespace Creature;
using namespace Creature::Animation;

CCreatureTreePanel::CCreatureTreePanel(
    const Creature::CCreature& creature,
    CCreatureEditor& editor,
    CEditorContext& context)
    : m_creature(creature)
    , m_editor(editor)
    , m_editorContext(context)
{
}

void CCreatureTreePanel::Draw()
{
    CImGuiWindowScope window("Creature");

    ImGui::PushID("Creature");

    const auto treeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
    const auto isOpen = ImGui::TreeNodeEx("##CreatureTree", treeFlags);

    ImGui::SameLine();

    if (m_isRenaming)
    {
        ImGui::SetNextItemWidth(150.0f);

        if (ImGui::InputText(
            "##CreatureName",
            m_szRenameBuffer,
            sizeof(m_szRenameBuffer),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_editor.SetName(m_szRenameBuffer);
            m_isRenaming = false;
        }
    }
    else
    {
        const bool isSelected = m_editorContext.GetSelectionType() == SelectionType::Creature;

        if (ImGui::Selectable(m_creature.GetName().c_str(), isSelected))
        {
            m_editorContext.SelectCreature();
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Rename"))
            {
                strcpy_s(m_szRenameBuffer, sizeof(m_szRenameBuffer), m_creature.GetName().c_str());
                m_isRenaming = true;
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
        for (const auto& entry : m_creature.GetAnimationEntries())
        {
            ImGui::PushID(entry.id);

            ImGuiTreeNodeFlags flags =
                ImGuiTreeNodeFlags_Leaf |
                ImGuiTreeNodeFlags_NoTreePushOnOpen |
                ImGuiTreeNodeFlags_SpanAvailWidth;

            if (m_editorContext.GetAnimationId() == entry.id)
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            ImGui::TreeNodeEx(entry.animation.GetName().c_str(), flags);

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                m_editorContext.SelectAnimation(entry.id);
            }

            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    ImGui::TreePop();
    ImGui::PopID();
}
