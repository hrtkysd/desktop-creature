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
        struct Vec2;
        class CMatrix3x2;
        class CTransformRect;        
    }
}

class CRenderPartItem;
class CTextureCache;

struct ImVec2;
struct ImDrawList;

enum class ResizeHandle : std::uint8_t;

class CPreviewPanel
{
public:
    void Draw(Creature::CCreature& creature, const Creature::CreaturePose& pose, CTextureCache& textureCache);
private:
    std::vector<CRenderPartItem> BuildPartViews(
        Creature::CCreature& creature,
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        CTextureCache& textureCache);
    void HandleInput(
        Creature::CCreature& creature,
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CRenderPartItem>& vecRenderPart);
    void DrawResizeHandle(ImDrawList* drawList, const Creature::Math::Vec2& position);
    void BeginOpeartion(Creature::CCreature& creature, const std::vector<CRenderPartItem>& vecPartView, const Creature::Math::Vec2& mousePosition);
    void UpdateOperation(Creature::CCreature& creature, const Creature::CreaturePose& pose, const std::vector<CRenderPartItem>& vecPartView, const Creature::Math::CMatrix3x2& previewTransform);
    void EndOperation();
    bool TryBeginResize(Creature::CCreature& creature, const CRenderPartItem& view, const Creature::Math::Vec2& mousePosition);

    void MovePart(
        Creature::CCreature& creature,
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
    void ResizePart(
        Creature::CCreature& creature,
        const Creature::CreaturePose& pose,
        const std::vector<CRenderPartItem>& vecPartView,
        const Creature::Math::CMatrix3x2& previewTransform);
private:
    Creature::PartId m_selectedPartId = Creature::INVALID_PART_ID;
    ResizeState m_resizeState{};
    Operation m_operation{};
    float m_fAnimationTime = 0.0f;
};
