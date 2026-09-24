#include "pch.h"
#include "Angle.h"
#include "CreaturePose.h"
#include "EditorContext.h"
#include "Part.h"
#include "PartTransformBuilder.h"
#include "PreviewEditor.h"
#include "PreviewPart.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"

#include "imgui.h"

#include <memory>

using namespace Creature;
using namespace Creature::Editor;
using namespace Creature::Math;

namespace
{
    Vec2 GetHandleDirection(
        ResizeHandle handle)
    {
        switch (handle)
        {
        case ResizeHandle::TopLeft:
            return { -1.0f, -1.0f };
        case ResizeHandle::TopRight:
            return { 1.0f, -1.0f };
        case ResizeHandle::BottomRight:
            return { 1.0f, 1.0f };
        case ResizeHandle::BottomLeft:
            return { -1.0f, 1.0f };
        default:
            return {};
        }
    }

    const CPreviewPart* FindPartView(
        const std::vector<CPreviewPart>& vecPartView,
        PartId partId)
    {
        if (partId == INVALID_PART_ID)  return nullptr;

        const auto itFind = std::find_if(
            vecPartView.begin(),
            vecPartView.end(),
            [partId](const CPreviewPart& view)
            {
                return view.GetPartId() == partId;
            });

        return itFind != vecPartView.end()
            ? std::addressof(*itFind)
            : nullptr;
    }

    const CPreviewPart* HitTestPart(
        const std::vector<CPreviewPart>& vecPreviewPart,
        const Vec2& mousePosition)
    {
        const auto itFind = std::find_if(
            vecPreviewPart.crbegin(),
            vecPreviewPart.crend(),
            [&mousePosition](const CPreviewPart& view)
            {
                return view.GetRect().Contains(mousePosition);
            });

        return itFind != vecPreviewPart.crend()
            ? std::addressof(*itFind)
            : nullptr;
    }

    bool IsInResizeHandle(
        const Vec2& point,
        const Vec2& handlePosition)
    {
        constexpr float halfSize = 6.0f;

        return
            point.x >= handlePosition.x - halfSize &&
            point.x <= handlePosition.x + halfSize &&
            point.y >= handlePosition.y - halfSize &&
            point.y <= handlePosition.y + halfSize;
    }

    ResizeHandle HitTestResizeHandle(
        const CTransformRect& rect,
        const Vec2& point)
    {
        const auto corner = rect.Corner();
        if (IsInResizeHandle(point, corner.topLeft)) return ResizeHandle::TopLeft;
        if (IsInResizeHandle(point, corner.topRight)) return ResizeHandle::TopRight;
        if (IsInResizeHandle(point, corner.bottomRight)) return ResizeHandle::BottomRight;
        if (IsInResizeHandle(point, corner.bottomLeft)) return ResizeHandle::BottomLeft;

        return ResizeHandle::None;
    }

    bool TryGetParentScreenInverse(
        const CSkeleton& skeleton,
        const Part& part,
        const CCreaturePose& pose,
        const CMatrix3x2& previewTransform,
        CMatrix3x2& inverse)
    {
        CMatrix3x2 parentWorld;
        if (part.parentId != INVALID_PART_ID)
        {
            if (const auto parent = skeleton.FindPartById(part.parentId))
            {
                parentWorld = CPartTransformBuilder::BuildWorld(*parent, skeleton, pose);
            }
        }

        const auto parentScreen = parentWorld * previewTransform;
        return parentScreen.TryInverse(inverse);
    }
}

CPreviewEditor::CPreviewEditor(
    CSkeletonEditor& editor,
    CEditorContext& context)
    : m_editor(editor)
    , m_editorContext(context)
{
}

const CEditorContext& CPreviewEditor::GetEditorContext() const noexcept
{
    return m_editorContext;
}

void CPreviewEditor::HandleInput(
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform,
    const std::vector<CPreviewPart>& vecPreviewPart,
    bool isInsidePreview)
{
    const auto mousePosition = ImGui::GetMousePos();

    if (isInsidePreview && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        BeginOperation(pose, previewTransform, vecPreviewPart, { mousePosition.x, mousePosition.y });
    }

    if (m_operation.eType != OperationType::None && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        UpdateOperation(pose, vecPreviewPart, previewTransform);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        EndOperation();
    }
}

void CPreviewEditor::Select(const CPreviewPart* view)
{
    m_editorContext.SelectPart(
        view
        ? view->GetPartId()
        : INVALID_PART_ID);
    m_operation = {};
}

void CPreviewEditor::BeginOperation(
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform,
    const std::vector<CPreviewPart>& vecPartView,
    const Vec2& mousePosition)
{
    switch (m_editorContext.GetEditMode())
    {
    case EditMode::Select:
        BeginSelect(vecPartView, mousePosition);
        return;
    case EditMode::Move:
        if (!BeginMove(vecPartView, mousePosition)) return;
        break;

    case EditMode::Scale:
        if (!BeginScale(vecPartView, mousePosition)) return;
        break;

    case EditMode::Rotate:
        if (!BeginRotate(pose, previewTransform, vecPartView, mousePosition)) return;
        break;

    case EditMode::Pivot:
        if (!BeginPivot(pose, previewTransform, vecPartView, mousePosition)) return;
        break;
    }
    m_undoScope.emplace(m_editorContext.CreateUndoScope());
}

void CPreviewEditor::UpdateOperation(
    const CCreaturePose& pose,
    const std::vector<CPreviewPart>& vecPartView,
    const CMatrix3x2& previewTransform)
{
    switch (m_operation.eType)
    {
    case OperationType::Move:
        MovePart(pose, previewTransform);
        break;
    case OperationType::Resize:
        ResizePart(pose, vecPartView, previewTransform);
        break;
    case OperationType::Rotate:
        RotatePart(pose, previewTransform);
        break;
    case OperationType::Pivot:
        MovePivot(pose, previewTransform);
        break;
    default:
        break;
    }
}

void CPreviewEditor::EndOperation()
{
    m_operation = {};
    m_undoScope.reset();
}

bool CPreviewEditor::BeginSelect(
    const std::vector<CPreviewPart>& vecPartView,
    const Vec2& mousePosition)
{
    Select(HitTestPart(vecPartView, mousePosition));
    return true;
}

bool CPreviewEditor::BeginMove(
    const std::vector<CPreviewPart>& vecPartView,
    const Vec2& mousePosition)
{
    const auto hitView = HitTestPart(vecPartView, mousePosition);
    if (!hitView)
    {
        Select(nullptr);
        return false;
    }

    Select(hitView);

    m_operation =
    {
        OperationType::Move,
        hitView->GetPartId()
    };
    return true;
}

bool CPreviewEditor::BeginScale(
    const std::vector<CPreviewPart>& vecPartView,
    const Vec2& mousePosition)
{
    const auto selectedView = FindPartView(vecPartView, m_editorContext.GetPartId());
    if (!selectedView)
    {
        Select(HitTestPart(vecPartView, mousePosition));
        return false;
    }

    const auto handle = HitTestResizeHandle(selectedView->GetRect(), mousePosition);
    if (handle == ResizeHandle::None)
    {
        Select(HitTestPart(vecPartView, mousePosition));
        return false;
    }

    const auto& skeleton = m_editor.GetSkeleton();
    auto part = skeleton.FindPartById(selectedView->GetPartId());
    if (!part) return false;

    m_operation =
    {
        OperationType::Resize,
        selectedView->GetPartId()
    };

    const auto& size = selectedView->GetSize();
    const auto direction = GetHandleDirection(handle);

    const Vec2 anchorLocal
    {
        -direction.x * size.x * 0.5f,
        -direction.y * size.y * 0.5f
    };

    const auto localTransform = part->bindTransform.ToMatrix();
    const auto anchor = localTransform.TransformPoint(anchorLocal);

    m_operation =
    {
        OperationType::Resize,
        part->id
    };

    m_resizeState.eHandle = handle;
    m_resizeState.anchor = anchor;

    return true;
}

bool CPreviewEditor::BeginRotate(
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform,
    const std::vector<CPreviewPart>& vecPartView,
    const Vec2& mousePosition)
{
    const auto hitView = HitTestPart(vecPartView, mousePosition);

    if (!hitView)
    {
        m_editorContext.SelectPart(INVALID_PART_ID);
        m_operation = {};
        return false;
    }

    m_editorContext.SelectPart(hitView->GetPartId());

    const auto& skeleton = m_editor.GetSkeleton();
    const auto part = skeleton.FindPartById(hitView->GetPartId());
    if (!part) return false;

    CMatrix3x2 parentWorld;
    if (part->parentId != INVALID_PART_ID)
    {
        if (const auto parent = skeleton.FindPartById(part->parentId))
        {
            parentWorld = CPartTransformBuilder::BuildWorld(*parent, skeleton, pose);
        }
    }

    const auto parentScreen = parentWorld * previewTransform;

    CMatrix3x2 inverseParentScreen;
    if (!parentScreen.TryInverse(inverseParentScreen)) return false;

    const auto mouseParent = inverseParentScreen.TransformPoint(
        {
            mousePosition.x,
            mousePosition.y
        });

    const auto& transform = part->bindTransform;
    const Vec2 pivotParent
    {
        transform.GetPosition().x + part->pivot.x,
        transform.GetPosition().y + part->pivot.y
    };

    const auto fAngle = Angle::Normalize(
        std::atan2(mouseParent.y - pivotParent.y, mouseParent.x - pivotParent.x));

    m_rotateState =
    {
        transform.GetRotation(),
        fAngle,
        pivotParent
    };

    m_operation =
    {
        OperationType::Rotate,
        part->id
    };

    return true;
}

bool CPreviewEditor::BeginPivot(
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform,
    const std::vector<CPreviewPart>& vecPartView,
    const Vec2& mousePosition)
{
    const auto selectedView = FindPartView(vecPartView, m_editorContext.GetPartId());
    if (!selectedView)
    {
        Select(HitTestPart(vecPartView, mousePosition));
        return false;
    }
    const auto& skeleton = m_editor.GetSkeleton();
    const auto part = skeleton.FindPartById(selectedView->GetPartId());
    if (!part) return false;

    const auto world = CPartTransformBuilder::BuildWorld(*part, skeleton, pose);

    const auto screen = world * previewTransform;
    const auto pivotScreen = screen.TransformPoint(part->pivot);

    constexpr float radius = 8.0f;

    const float dx = mousePosition.x - pivotScreen.x;
    const float dy = mousePosition.y - pivotScreen.y;

    if (dx * dx + dy * dy > radius * radius)
    {
        Select(HitTestPart(vecPartView, mousePosition));
        return false;
    }
    m_operation =
    {
        OperationType::Pivot,
        selectedView->GetPartId()
    };
    return true;
}

void CPreviewEditor::MovePart(
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform)
{
    const auto& skeleton = m_editor.GetSkeleton();

    auto part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    CMatrix3x2 inverse;
    if (!TryGetParentScreenInverse(skeleton, *part, pose, previewTransform, inverse))
    {
        return;
    }

    const auto mouse = ImGui::GetMousePos();
    const auto delta = ImGui::GetIO().MouseDelta;

    const Vec2 currentMouse = inverse.TransformPoint(
        {
            mouse.x,
            mouse.y
        });

    const Vec2 previousMouse = inverse.TransformPoint(
        {
            mouse.x - delta.x,
            mouse.y - delta.y
        });

    auto transform = part->bindTransform;

    transform.GetPosition().x += currentMouse.x - previousMouse.x;
    transform.GetPosition().y += currentMouse.y - previousMouse.y;

    m_editor.SetPartTransform(part->id, transform);
}

void CPreviewEditor::ResizePart(
    const Creature::CCreaturePose& pose,
    const std::vector<CPreviewPart>& vecPartView,
    const CMatrix3x2& previewTransform)
{
    const auto& skeleton = m_editor.GetSkeleton();

    auto part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    const auto view = FindPartView(vecPartView, m_operation.partId);
    if (!view) return;

    CMatrix3x2 inverse;
    if (!TryGetParentScreenInverse(skeleton, *part, pose, previewTransform, inverse))
    {
        return;
    }

    const auto mouse = ImGui::GetMousePos();
    const auto mouseParent = inverse.TransformPoint(
        {
            mouse.x,
            mouse.y
        });

    const Vec2 delta
    {
        mouseParent.x - m_resizeState.anchor.x,
        mouseParent.y - m_resizeState.anchor.y
    };

    auto editTransform = part->bindTransform;
    const auto inverseRotation = CMatrix3x2::CreateRotation(-editTransform.GetRotation());

    const auto localDelta = inverseRotation.TransformPoint(delta);
    const auto direction = GetHandleDirection(m_resizeState.eHandle);

    const auto& size = view->GetSize();
    Vec2 newScale
    {
        localDelta.x / (direction.x * size.x),
        localDelta.y / (direction.y * size.y)
    };

    constexpr float minScale = 0.05f;

    newScale.x = std::max(newScale.x, minScale);
    newScale.y = std::max(newScale.y, minScale);

    const Vec2 anchorLocal
    {
        -direction.x * size.x * 0.5f,
        -direction.y * size.y * 0.5f
    };

    const auto scaleRotation =
        CMatrix3x2::CreateScale(newScale)
        *
        CMatrix3x2::CreateRotation(editTransform.GetRotation());

    const auto anchorOffset = scaleRotation.TransformPoint(anchorLocal);

    Vec2 newPosition
    {
        m_resizeState.anchor.x - anchorOffset.x,
        m_resizeState.anchor.y - anchorOffset.y
    };

    editTransform.GetScale() = newScale;
    editTransform.GetPosition() = newPosition;

    m_editor.SetPartTransform(part->id, editTransform);
}

void CPreviewEditor::RotatePart(
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform)
{
    const auto& skeleton = m_editor.GetSkeleton();

    const auto part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    CMatrix3x2 inverse;
    if (!TryGetParentScreenInverse(skeleton, *part, pose, previewTransform, inverse))
    {
        return;
    }

    const auto mousePosition = ImGui::GetMousePos();
    const auto mouseParent = inverse.TransformPoint(
        {
            mousePosition.x,
            mousePosition.y
        });

    const auto fCurrentAngle = Angle::Normalize(
        std::atan2(
            mouseParent.y - m_rotateState.pivot.y,
            mouseParent.x - m_rotateState.pivot.x));

    const auto fDelta = fCurrentAngle - m_rotateState.fMouseAngleStart;

    auto transform = part->bindTransform;
    transform.SetRotation(m_rotateState.fPartRotationStart + fDelta);

    m_editor.SetPartTransform(part->id, transform);
}

void CPreviewEditor::MovePivot(
    const CCreaturePose& pose,
    const CMatrix3x2& previewTransform)
{
    const auto& skeleton = m_editor.GetSkeleton();
    const auto part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    CMatrix3x2 inverse;
    if (!TryGetParentScreenInverse(skeleton, *part, pose, previewTransform, inverse))
    {
        return;
    }

    const auto mouse = ImGui::GetMousePos();
    const Vec2 mouseParent = inverse.TransformPoint(
        {
            mouse.x,
            mouse.y
        });
    const auto local = CPartTransformBuilder::BuildLocal(*part, part->bindTransform);
    CMatrix3x2 inverseLocal;
    if (!local.TryInverse(inverseLocal)) return;

    const Vec2 newPivot = inverseLocal.TransformPoint(mouseParent);
    const Vec2 deltaPivot
    {
        newPivot.x - part->pivot.x,
        newPivot.y - part->pivot.y
    };

    const auto scaleRotation = CMatrix3x2::CreateScale(
        part->bindTransform.GetScale())
        *
        CMatrix3x2::CreateRotation(part->bindTransform.GetRotation());

    const auto transformedDelta = scaleRotation.TransformPoint(deltaPivot);

    auto transform = part->bindTransform;
    auto& position = transform.GetPosition();
    position.x += transformedDelta.x - deltaPivot.x;
    position.y += transformedDelta.y - deltaPivot.y;

    m_editor.SetPartPivotAndTransform(part->id, newPivot, transform);
}
