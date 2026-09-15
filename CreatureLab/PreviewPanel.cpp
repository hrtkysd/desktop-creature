#include "pch.h"
#include "Appearance.h"
#include "Creature.h"
#include "ImGuiWindowScope.h"
#include "PreviewPanel.h"
#include "RectCorner.h"
#include "RenderPartItem.h"
#include "Skeleton.h"
#include "Texture.h"
#include "TextureCache.h"
#include "TransformRect.h"

#include "imgui.h"

using namespace Creature;
using namespace Creature::Math;

namespace
{
    CMatrix3x2 BuildWorldTransform(
        const Part& part,
        const CSkeleton& skeleton)
    {
        const auto local = part.bindTransform.ToMatrix();

        if (part.parentId == INVALID_PART_ID) return local;

        const auto parent = skeleton.FindPartById(part.parentId);
        if (!parent) return local;

        return local * BuildWorldTransform(*parent, skeleton);
    }
}

void CPreviewPanel::Draw(CCreature& creature, CTextureCache& textureCache)
{
    CImGuiWindowScope scope("Preview");

    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 area = ImGui::GetContentRegionAvail();

    auto* drawList = ImGui::GetWindowDrawList();

    const ImVec2 center
    {
        origin.x + area.x * 0.5f,
        origin.y + area.y * 0.5f
    };

    constexpr auto scale = 0.25f;
    const auto vecPartView = BuildPartViews(creature, textureCache, center, scale);
   
    HandleInput(creature, vecPartView);

    for (const auto& part : vecPartView)
    {
        const auto corners = part.GetRect().Corners();
        drawList->AddImageQuad(
            part.GetTexture()->GetShaderResourceView(),
            ImVec2{ corners.topLeft.x,     corners.topLeft.y },
            ImVec2{ corners.topRight.x,    corners.topRight.y },
            ImVec2{ corners.bottomRight.x, corners.bottomRight.y },
            ImVec2{ corners.bottomLeft.x,  corners.bottomLeft.y });
        const float renderedWidth =
            std::hypot(
                corners.topRight.x - corners.topLeft.x,
                corners.topRight.y - corners.topLeft.y);

        const float renderedHeight =
            std::hypot(
                corners.bottomLeft.x - corners.topLeft.x,
                corners.bottomLeft.y - corners.topLeft.y);
    }

    ImGui::Dummy(area);
}

void CPreviewPanel::HandleInput(CCreature& creature, const std::vector<CRenderPartItem>& vecPartView)
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        m_selectedPartId = INVALID_PART_ID;
        const auto mousePos = ImGui::GetMousePos();
        const auto itFind = std::find_if(vecPartView.crbegin(), vecPartView.crend(), [&mousePos](const CRenderPartItem& view) {
            return view.IsPtInView({mousePos.x, mousePos.y});
        });
        if (itFind != vecPartView.crend())
            m_selectedPartId = itFind->GetPartId();
    }

    if (m_selectedPartId == INVALID_PART_ID) return;

    const auto part = creature.GetSkeleton().FindPartById(m_selectedPartId);
    if (!part) return;

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        constexpr float scale = 0.25f;

        const ImVec2 delta = ImGui::GetIO().MouseDelta;
        auto& position = part->bindTransform.GetPosition();
        position.x += delta.x / scale;
        position.y += delta.y / scale;
    }
}

std::vector<CRenderPartItem> CPreviewPanel::BuildPartViews(Creature::CCreature& creature, CTextureCache& textureCache, const ImVec2& center, float scale)
{
    const auto& skeleton = creature.GetSkeleton();

    std::vector<CRenderPartItem> result;
    result.reserve(skeleton.Parts().size());

    const auto previewTransform =
        CMatrix3x2::CreateScale(
        {
            scale,
            scale
        })
        *
        CMatrix3x2::CreateTranslation(
        {
            center.x,
            center.y
        });

    for (const auto& part : skeleton.Parts())
    {
        const auto appearance = creature.GetAppearance().FindByPartId(part.id);
        if (!appearance) continue;

        auto texture = textureCache.Load(appearance->texturePath);
        if (!texture) continue;

        const auto worldTransform = BuildWorldTransform(part, skeleton);
        const auto screenTransform = worldTransform * previewTransform;

        const Vec2 size
        {
            static_cast<float>(texture->GetWidth()),
            static_cast<float>(texture->GetHeight())
        };

        CTransformRect rect(size, screenTransform);

        result.emplace_back(
            part.id,
            std::move(rect),
            std::move(texture));
    }

    return result;
}
