#include "pch.h"
#include "Appearance.h"
#include "CreatureEditor.h"
#include "CreaturePose.h"
#include "EditorContext.h"
#include "ImGuiWindowScope.h"
#include "PartTransformBuilder.h"
#include "PreviewPanel.h"
#include "PreviewPart.h"
#include "RectCorner.h"
#include "RenderPartItem.h"
#include "RenderTarget.h"
#include "Skeleton.h"
#include "SpriteRenderDescription.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "TextureCache.h"
#include "TransformRect.h"

#include "imgui.h"

using namespace Creature;
using namespace Creature::Editor;
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

    bool IsMouseCursorInPanel(const ImVec2& imageOrigin, const ImVec2& imageSize)
    {
        const auto mouse = ImGui::GetMousePos();
        return
            mouse.x >= imageOrigin.x &&
            mouse.x <= imageOrigin.x + imageSize.x &&
            mouse.y >= imageOrigin.y &&
            mouse.y <= imageOrigin.y + imageSize.y;
    }
}

CPreviewPanel::CPreviewPanel(
    CCreatureEditor& editor,
    CEditorContext& context)
    : m_previewEditor(editor, context)
{
}

void CPreviewPanel::DrawUi(
    const CSkeleton& skeleton,
    const CAppearance& appearance,
    const CCreaturePose& pose,
    CTextureCache& textureCache,
    const CRenderTarget& renderTarget)
{
    CImGuiWindowScope scope("Preview");

    const ImVec2 panelOrigin = ImGui::GetCursorScreenPos();
    const ImVec2 panelArea = ImGui::GetContentRegionAvail();

    m_contentSize =
    {
        std::max(0.0f, panelArea.x),
        std::max(0.0f, panelArea.y)
    };

    const Vec2 previewSize
    {
        static_cast<float>(renderTarget.Width()),
        static_cast<float>(renderTarget.Height())
    };

    constexpr float creatureScale = 0.25f;

    const Vec2 previewCenter
    {
        previewSize.x * 0.5f,
        previewSize.y * 0.5f
    };

    const auto creatureToPreview =
        CMatrix3x2::CreateScale({ creatureScale, creatureScale }) *
        CMatrix3x2::CreateTranslation(previewCenter);

    m_vecRenderItem = BuildPartViews(
        skeleton,
        appearance,
        pose,
        creatureToPreview,
        textureCache);

    const float imageScale = std::min(
        panelArea.x / previewSize.x,
        panelArea.y / previewSize.y);

    const ImVec2 imageSize
    {
        previewSize.x * imageScale,
        previewSize.y * imageScale
    };

    const ImVec2 imageOrigin
    {
        panelOrigin.x + (panelArea.x - imageSize.x) * 0.5f,
        panelOrigin.y + (panelArea.y - imageSize.y) * 0.5f
    };

    ImGui::SetCursorScreenPos(imageOrigin);
    ImGui::Image(reinterpret_cast<ImTextureID>(renderTarget.SRV()), imageSize);

    const auto previewToScreen =
        CMatrix3x2::CreateScale({ imageScale, imageScale }) *
        CMatrix3x2::CreateTranslation({ imageOrigin.x, imageOrigin.y });

    const auto creatureToScreen =
        creatureToPreview * previewToScreen;
    std::vector<CPreviewPart> vecPreviewPart;
    vecPreviewPart.reserve(m_vecRenderItem.size());

    for (const auto& item : m_vecRenderItem)
    {
        CTransformRect screenRect(
            item.GetRect().Size(),
            item.GetRect().Matrix() * previewToScreen);

        vecPreviewPart.emplace_back(
            item.GetPartId(),
            screenRect,
            Vec2
            {
                static_cast<float>(item.GetTexture()->GetWidth()),
                static_cast<float>(item.GetTexture()->GetHeight())
            });
    }

    m_previewEditor.HandleInput(
        pose,
        creatureToScreen,
        vecPreviewPart,
        IsMouseCursorInPanel(imageOrigin, imageSize));

    const auto& editorContext = m_previewEditor.GetEditorContext();
    auto drawList = ImGui::GetWindowDrawList();

    for (const auto& item : m_vecRenderItem)
    {
        if (editorContext.GetPartId() != item.GetPartId())
            continue;

        CTransformRect screenRect(
            item.GetRect().Size(),
            item.GetRect().Matrix() * previewToScreen);

        const auto corner = screenRect.Corner();

        DrawSelectPartFrameRect(drawList, corner, item);

        if (editorContext.GetEditMode() == EditMode::Scale)
        {
            DrawResizeHandle(drawList, corner.topLeft);
            DrawResizeHandle(drawList, corner.topRight);
            DrawResizeHandle(drawList, corner.bottomRight);
            DrawResizeHandle(drawList, corner.bottomLeft);
        }
        else if (editorContext.GetEditMode() == EditMode::Pivot)
        {
            const auto part = skeleton.FindPartById(item.GetPartId());
            if (part)
            {
                const auto worldTransform =
                    CPartTransformBuilder::BuildWorld(
                        *part,
                        skeleton,
                        pose);

                const auto screenTransform =
                    worldTransform * creatureToScreen;
                const auto pivotScreen =
                    screenTransform.TransformPoint(part->pivot);

                DrawPivotHandle(drawList, pivotScreen);
            }
        }
    }
}

void CPreviewPanel::RenderPreview(CSpriteRenderer& renderer)
{
    for (const auto& partView : m_vecRenderItem)
    {
        CSpriteRenderDescription desc
        {
            partView.GetTexture(),
            partView.GetRect().Size(),
            partView.GetRect().Matrix()
        };

        renderer.Draw(desc);
    }
}

const Vec2& CPreviewPanel::GetPreviewContentSize() const
{
    return m_contentSize;
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
    const CSkeleton& skeleton,
    const CAppearance& appearance,
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform,
    CTextureCache& textureCache)
{
    std::vector<CRenderPartItem> result;
    result.reserve(skeleton.Parts().size());

    for (const auto& part : appearance.Parts())
    {
        const auto skeletonPart = skeleton.FindPartById(part.partId);
        if (!skeletonPart) continue;

        auto texture = textureCache.Load(part.texturePath);
        if (!texture) continue;

        const auto worldTransform = CPartTransformBuilder::BuildWorld(*skeletonPart, skeleton, pose);
        const auto screenTransform = worldTransform * previewTransform;

        const Vec2 size
        {
            static_cast<float>(texture->GetWidth()),
            static_cast<float>(texture->GetHeight())
        };

        CTransformRect rect(size, screenTransform);

        result.emplace_back(
            part.partId,
            rect,
            std::move(texture));
    }

    return result;
}
