#pragma once

#include "Part.h"
#include "Operation.h"
#include "Resize.h"

namespace Creature
{
    class CCreature;

    struct CreaturePose;

    namespace Math
    {
        struct RectCorner;
        struct Vec2;

        class CMatrix3x2;
        class CTransformRect;
    }
}

class CEditorContext;
class CLabController;
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
    void HandleInput(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CRenderPartItem>& vecRenderPart);
    void DrawResizeHandle(
        ImDrawList* drawList,
        const Creature::Math::Vec2& position);
    void BeginOpeartion(
        const std::vector<CRenderPartItem>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void UpdateOperation(
        const Creature::CreaturePose& pose,
        const std::vector<CRenderPartItem>& vecPartView,
        const Creature::Math::CMatrix3x2& previewTransform);
    void EndOperation();

    bool TryBeginResize(
        const CRenderPartItem& view,
        const Creature::Math::Vec2& mousePosition);

    void MovePart(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
    void ResizePart(
        const Creature::CreaturePose& pose,
        const std::vector<CRenderPartItem>& vecPartView,
        const Creature::Math::CMatrix3x2& previewTransform);
private:
    const Creature::CCreature& m_creature;
    CCreatureEditor& m_editor;
    CEditorContext& m_editorContext;
    ResizeState m_resizeState{};
    Operation m_operation{};
};
