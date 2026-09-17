#include "pch.h"
#include "Appearance.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreaturePose.h"
#include "EditorContext.h"
#include "ImGuiWindowScope.h"
#include "Part.h"
#include "PartTransformBuilder.h"
#include "PreviewPanel.h"
#include "PreviewPart.h"
#include "RectCorner.h"
#include "RenderPartItem.h"
#include "Skeleton.h"
#include "Texture.h"
#include "TextureCache.h"
#include "TransformRect.h"
#include "Vec2.h"

#include "imgui.h"

using namespace Creature;
using namespace Creature::Math;

namespace
{
    CPreviewPart ToPreviewPart(const CRenderPartItem& item)
    {
        return CPreviewPart
        {
            item.GetPartId(),
            item.GetRect(),
            Vec2
            {
                static_cast<float>(item.GetTexture()->GetWidth()),
                static_cast<float>(item.GetTexture()->GetHeight()),
            }
        };
    }
}

CPreviewPanel::CPreviewPanel(
    const Creature::CCreature& creature,
    CCreatureEditor& editor,
    CEditorContext& context)
    : m_creature(creature)
    , m_editorContext(context)
    , m_previewEditor(creature, editor, context)
{
}

void CPreviewPanel::Draw(
    const CreaturePose& pose,
    CTextureCache& textureCache)
{
    CImGuiWindowScope scope("Preview");

    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 area = ImGui::GetContentRegionAvail();

    const ImVec2 center
    {
        origin.x + area.x * 0.5f,
        origin.y + area.y * 0.5f
    };

    constexpr float scale = 0.25f;

    const auto previewTransform =
        CMatrix3x2::CreateScale({ scale, scale })
        *
        CMatrix3x2::CreateTranslation({
            center.x,
            center.y
            });

    const auto vecPartView = BuildPartViews(pose, previewTransform, textureCache);

    std::vector<CPreviewPart> vecEditPreview;
    vecEditPreview.reserve(vecPartView.size());
    std::transform(vecPartView.begin(), vecPartView.end(), std::back_inserter(vecEditPreview), [](const CRenderPartItem& renderItem) {
        return ToPreviewPart(renderItem);
    });
    m_previewEditor.HandleInput(pose, previewTransform, vecEditPreview);

    const auto drawList = ImGui::GetWindowDrawList();
    for (const auto& partView : vecPartView)
    {
        const auto corner = partView.GetRect().Corner();
        drawList->AddImageQuad(
            partView.GetTexture()->GetShaderResourceView(),
            ImVec2{ corner.topLeft.x,     corner.topLeft.y },
            ImVec2{ corner.topRight.x,    corner.topRight.y },
            ImVec2{ corner.bottomRight.x, corner.bottomRight.y },
            ImVec2{ corner.bottomLeft.x,  corner.bottomLeft.y });

        if (m_editorContext.GetPartId() == partView.GetPartId())
        {
            DrawSelectPartFrameRect(drawList, corner, partView);

            if (m_editorContext.GetEditMode() == EditMode::Scale)
            {
                DrawResizeHandle(drawList, corner.topLeft);
                DrawResizeHandle(drawList, corner.topRight);
                DrawResizeHandle(drawList, corner.bottomRight);
                DrawResizeHandle(drawList, corner.bottomLeft);
            }
            else if (m_editorContext.GetEditMode() == EditMode::Pivot)
            {
                const auto& skeleton = m_creature.GetSkeleton();
                const auto part = skeleton.FindPartById(partView.GetPartId());
                if (part)
                {
                    const auto worldTransform =
                        CPartTransformBuilder::BuildWorld(
                            *part,
                            skeleton,
                            pose);

                    const auto screenTransform =
                        worldTransform * previewTransform;

                    const auto pivotScreen =
                        screenTransform.TransformPoint(part->pivot);
                    DrawPivotHandle(drawList, pivotScreen);
                }
            }
        }
    }
    ImGui::Dummy(area);
}

void CPreviewPanel::DrawResizeHandle(
    ImDrawList* drawList,
    const Vec2& position)
{
    constexpr float halfSize = 4.0f;

    drawList->AddRectFilled(
        ImVec2{
            position.x - halfSize,
            position.y - halfSize
        },
        ImVec2{
            position.x + halfSize,
            position.y + halfSize
        },
        IM_COL32(255, 255, 255, 255));

    drawList->AddRect(
        ImVec2{
            position.x - halfSize,
            position.y - halfSize
        },
        ImVec2{
            position.x + halfSize,
            position.y + halfSize
        },
        IM_COL32(255, 0, 0, 255));
}

void CPreviewPanel::DrawPivotHandle(ImDrawList* drawList, const Vec2& position)
{
    constexpr float radius = 5.0f;

    drawList->AddCircleFilled(
        { position.x, position.y },
        radius,
        IM_COL32(255, 255, 255, 255));

    drawList->AddCircle(
        { position.x, position.y },
        radius,
        IM_COL32(255, 0, 0, 255),
        0,
        2.0f);
}

void CPreviewPanel::DrawSelectPartFrameRect(
    ImDrawList* drawList,
    const RectCorner& corner,
    const CRenderPartItem& selectPartView)
{
    const ImU32 color = IM_COL32(255, 0, 0, 255);
    drawList->AddLine(
        ImVec2{ corner.topLeft.x, corner.topLeft.y },
        ImVec2{ corner.topRight.x, corner.topRight.y },
        color,
        2.0f);
    drawList->AddLine(
        ImVec2{ corner.topRight.x, corner.topRight.y },
        ImVec2{ corner.bottomRight.x, corner.bottomRight.y },
        color,
        2.0f);
    drawList->AddLine(
        ImVec2{ corner.bottomRight.x, corner.bottomRight.y },
        ImVec2{ corner.bottomLeft.x, corner.bottomLeft.y },
        color,
        2.0f);
    drawList->AddLine(
        ImVec2{ corner.bottomLeft.x, corner.bottomLeft.y },
        ImVec2{ corner.topLeft.x, corner.topLeft.y },
        color,
        2.0f);
}

std::vector<CRenderPartItem> CPreviewPanel::BuildPartViews(
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform,
    CTextureCache& textureCache)
{
    const auto& skeleton = m_creature.GetSkeleton();

    std::vector<CRenderPartItem> result;
    result.reserve(skeleton.Parts().size());

    for (const auto& part : skeleton.Parts())
    {
        const auto appearance = m_creature.GetAppearance().FindByPartId(part.id);
        if (!appearance) continue;

        auto texture = textureCache.Load(appearance->texturePath);
        if (!texture) continue;

        const auto worldTransform = CPartTransformBuilder::BuildWorld(part, skeleton, pose);
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
