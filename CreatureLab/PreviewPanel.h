#pragma once

#include "PreviewEditor.h"
#include "RenderPartItem.h"
#include "Vec2.h"

namespace Creature
{
    class CAppearance;
    class CCreaturePose;
    class CSkeleton;

    namespace Math
    {
        struct RectCorner;

        class CMatrix3x2;
    }
}

class CEditorContext;
class CSpriteRenderer;
class CTextureCache;
class CRenderTarget;

struct ImDrawList;

enum class ResizeHandle : std::uint8_t;

class CPreviewPanel
{
public:
    explicit CPreviewPanel(
        Creature::Editor::CCreatureEditor& editor,
        CEditorContext& context);
public:
    void DrawUi(
        const Creature::CSkeleton& skeleton,
        const Creature::CAppearance& appearance,
        const Creature::CCreaturePose& pose,
        CTextureCache& textureCache,
        const CRenderTarget& renderTarget);
    void RenderPreview(CSpriteRenderer& renderer);
    const Creature::Math::Vec2& GetPreviewContentSize() const;
private:
    void DrawSelectPartFrameRect(
        ImDrawList* drawList,
        const Creature::Math::RectCorner& corner,
        const CRenderPartItem& selectPartView);
    std::vector<CRenderPartItem> BuildPartViews(
        const Creature::CSkeleton& skeleton,
        const Creature::CAppearance& appearance,
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        CTextureCache& textureCache);
    void DrawResizeHandle(
        ImDrawList* drawList,
        const Creature::Math::Vec2& position);
    void DrawPivotHandle(
        ImDrawList* drawList,
        const Creature::Math::Vec2& position);
private:
    CPreviewEditor m_previewEditor;
    Creature::Math::Vec2 m_contentSize{};
    std::vector<CRenderPartItem> m_vecRenderItem;
};
