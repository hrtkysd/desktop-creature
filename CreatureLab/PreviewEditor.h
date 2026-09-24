#pragma once

#include "Operation.h"
#include "Resize.h"
#include "Rotate.h"
#include "UndoScope.h"

#include <optional>
#include <vector>

namespace Creature
{
    class CCreature;
    class CCreaturePose;
    namespace Editor
    {
        class CSkeletonEditor;
    }
    namespace Math
    {
        struct RectCorner;
        struct Vec2;

        class CMatrix3x2;
        class CTransformRect;
    }
}

class CPreviewPart;
class CEditorContext;

class CPreviewEditor
{
public:
    explicit CPreviewEditor(
        Creature::Editor::CSkeletonEditor& editor,
        CEditorContext& context);

public:
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

    bool BeginSelect(
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    bool BeginMove(
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    bool BeginScale(
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    bool BeginRotate(
        const Creature::CCreaturePose& pose,
        const Creature::Math::CMatrix3x2& previewTransform,
        const std::vector<CPreviewPart>& vecPartView,
        const Creature::Math::Vec2& mousePosition);
    bool BeginPivot(
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
    Creature::Editor::CSkeletonEditor& m_editor;
    CEditorContext& m_editorContext;

    std::optional<CUndoScope> m_undoScope;

    Operation m_operation{};
    ResizeState m_resizeState{};
    RotateState m_rotateState{};
};
