#include "pch.h"
#include "Genome.h"
#include "PreviewPanel.h"

#include "imgui.h"

using namespace Creature;

void CPreviewPanel::Draw(const Genome& genome)
{
    ImGui::Begin("Preview");

    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 area = ImGui::GetContentRegionAvail();

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    const ImVec2 center{
        origin.x + area.x * 0.5f,
        origin.y + area.y * 0.5f
    };

    const float bodyWidth = 150.0f * genome.bodyLength;
    const float bodyHeight = 80.0f;

    const ImVec2 bodyMin{
        center.x - bodyWidth * 0.5f,
        center.y - bodyHeight * 0.5f
    };

    const ImVec2 bodyMax{
        center.x + bodyWidth * 0.5f,
        center.y + bodyHeight * 0.5f
    };

    drawList->AddEllipseFilled(
        center,
        ImVec2{ bodyWidth * 0.5f, bodyHeight * 0.5f },
        IM_COL32(180, 180, 180, 255));

    const float headRadius = 40.0f * genome.headScale;

    const ImVec2 headCenter{
        bodyMax.x + headRadius * 0.6f,
        center.y - 10.0f
    };

    drawList->AddCircleFilled(
        headCenter,
        headRadius,
        IM_COL32(210, 210, 210, 255));

    const float legLength = 60.0f * genome.legLength;

    drawList->AddLine(
        ImVec2{ center.x - bodyWidth * 0.25f, bodyMax.y },
        ImVec2{ center.x - bodyWidth * 0.25f, bodyMax.y + legLength },
        IM_COL32(220, 220, 220, 255),
        8.0f);

    drawList->AddLine(
        ImVec2{ center.x + bodyWidth * 0.25f, bodyMax.y },
        ImVec2{ center.x + bodyWidth * 0.25f, bodyMax.y + legLength },
        IM_COL32(220, 220, 220, 255),
        8.0f);

    ImGui::Dummy(area);

    ImGui::End();
}
