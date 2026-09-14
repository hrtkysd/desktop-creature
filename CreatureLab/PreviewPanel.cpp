#include "pch.h"
#include "Genome.h"
#include "ImGuiWindowScope.h"
#include "PreviewPanel.h"
#include "Texture.h"
#include "TextureCache.h"
#include "imgui.h"

using namespace Creature;

void CPreviewPanel::Draw(const Genome& genome, const std::shared_ptr<CTextureCache>& textureCache)
{
    CImGuiWindowScope scope("Preview");

    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 area = ImGui::GetContentRegionAvail();

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    const ImVec2 center
    {
        origin.x + area.x * 0.5f,
        origin.y + area.y * 0.5f
    };

    const auto body = textureCache->Load(L"assets/body.png");

    if (!body) return;

    const float scale = 0.25f;

    const float bodyWidth =
        static_cast<float>(body->GetWidth()) * scale;
    const float bodyHeight =
        static_cast<float>(body->GetHeight()) * scale;

    const ImVec2 bodyMin{
        center.x - bodyWidth * 0.5f,
        center.y - bodyHeight * 0.5f
    };
    const ImVec2 bodyMax{
        center.x + bodyWidth * 0.5f,
        center.y + bodyHeight * 0.5f
    };
    drawList->AddImage(
        body->GetShaderResourceView(),
        bodyMin,
        bodyMax);
}
