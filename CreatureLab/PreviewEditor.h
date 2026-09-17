#pragma once

#include "Operation.h"
#include "Resize.h"
#include "Rotate.h"

#include <vector>

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

class CPreviewPart;
class CCreatureEditor;
class CEditorContext;

class CPreviewEditor
{
public:
    explicit CPreviewEditor(
        const Creature::CCreature& creature,
        CCreatureEditor& editor,
        CEditorContext& context);

public:
    void HandleInput(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPreview);

private:
    void Select(const CPreviewPart* view);
    void BeginOperation(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void UpdateOperation(
        const Creature::CreaturePose& pose,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::CMatrix3x2& previewTransform);
    void EndOperation();

    void BeginSelect(
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void BeginMove(
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void BeginScale(
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void BeginRotate(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void BeginPivot(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);

    void MovePart(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
    void ResizePart(
        const Creature::CreaturePose& pose,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::CMatrix3x2& previewTransform);
    void RotatePart(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
    void MovePivot(
        const Creature::CreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
private:
    const Creature::CCreature& m_creature;
    CCreatureEditor& m_editor;
    CEditorContext& m_editorContext;

    Operation m_operation{};
    ResizeState m_resizeState{};
    RotateState m_rotateState{};
};
