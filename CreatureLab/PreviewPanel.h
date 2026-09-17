#pragma once

#include "PreviewEditor.h"

namespace Creature
{
    class CCreature;

    struct CreaturePose;

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
        const Creature::CCreature& creature,
        CCreatureEditor& editor,
        CEditorContext& context);
public:
    void Draw(
        const Creature::CreaturePose& pose,
        CTextureCache& textureCache);
private:
    void DrawSelectPartFrameRect(
        ImDrawList* drawList,
        const Creature::Math::RectCorner& corner,
        const CRenderPartItem& selectPartView);
    std::vector<CRenderPartItem> BuildPartViews(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        CTextureCache& textureCache);
    void DrawResizeHandle(
        ImDrawList* drawList,
        const Creature::Math::Vec2& position);
    void DrawPivotHandle(
        ImDrawList* drawList,
        const Creature::Math::Vec2& position);
private:
    const Creature::CCreature& m_creature;
    CEditorContext& m_editorContext;
    CPreviewEditor m_previewEditor;
};
