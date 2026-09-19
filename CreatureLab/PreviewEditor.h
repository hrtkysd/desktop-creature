#pragma once

#include "Operation.h"
#include "Resize.h"
#include "Rotate.h"

#include <vector>

namespace Creature
{
    class CCreature;
    class CCreaturePose;

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
        CCreatureEditor& editor,
        CEditorContext& context);

public:
    const CCreatureEditor& GetEditor() const noexcept;
    const CEditorContext& GetEditorContext() const noexcept;

    void HandleInput(
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPreview,
        bool isInsidePreview);

private:
    void Select(const CPreviewPart* view);
    void BeginOperation(
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void UpdateOperation(
        const Creature::CCreaturePose& pose,
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
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    void BeginPivot(
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);

    void MovePart(
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
    void ResizePart(
        const Creature::CCreaturePose& pose,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::CMatrix3x2& previewTransform);
    void RotatePart(
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
    void MovePivot(
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform);
private:
    CCreatureEditor& m_editor;
    CEditorContext& m_editorContext;

    Operation m_operation{};
    ResizeState m_resizeState{};
    RotateState m_rotateState{};
};
