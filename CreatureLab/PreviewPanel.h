#pragma once

#include "PreviewEditor.h"

namespace Creature
{
    class CCreature;
    class CCreaturePose;

    namespace Math
    {
        struct RectCorner;
        struct Vec2;

        class CMatrix3x2;
    }
}

class CCreatureEditor;
class CEditorContext;
class CRenderPartItem;
class CTextureCache;

struct ImDrawList;

enum class ResizeHandle : std::uint8_t;

class CPreviewPanel
{
public:
    explicit CPreviewPanel(
        CCreatureEditor& editor,
        CEditorContext& context);
public:
    void Draw(
        const Creature::CCreaturePose& pose,
        CTextureCache& textureCache);
private:
    void DrawSelectPartFrameRect(
        ImDrawList* drawList,
        const Creature::Math::RectCorner& corner,
        const CRenderPartItem& selectPartView);
    std::vector<CRenderPartItem> BuildPartViews(
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
};
