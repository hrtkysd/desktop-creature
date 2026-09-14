#include "pch.h"
#include "Genome.h"
#include "GenomePanel.h"
#include "ImGuiWindowScope.h"

#include "imgui.h"

using namespace Creature;

bool CGenomePanel::Draw(Genome& genome)
{
    bool changed = false;

    CImGuiWindowScope window("Genome");

    if (!window) return false;

    changed |= ImGui::SliderFloat(
        "Body Length",
        &genome.bodyLength,
        0.5f,
        1.5f);

    changed |= ImGui::SliderFloat(
        "Head Scale",
        &genome.headScale,
        0.5f,
        1.5f);

    return changed;
}
