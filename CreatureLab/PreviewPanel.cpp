#include "pch.h"
#include "Angle.h"
#include "Animation.h"
#include "Appearance.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreaturePose.h"
#include "EditorContext.h"
#include "ImGuiWindowScope.h"
#include "LabController.h"
#include "PartTransformBuilder.h"
#include "PreviewPanel.h"
#include "RectCorner.h"
#include "RenderPartItem.h"
#include "Skeleton.h"
#include "Texture.h"
#include "TextureCache.h"
#include "TransformRect.h"
#include "Vec2.h"

#include "imgui.h"

using namespace Creature;
using namespace Creature::Animation;
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

    const CRenderPartItem* FindPartView(
        const std::vector<CRenderPartItem>& vecPartView,
        PartId partId)
    {
        if (partId == INVALID_PART_ID)
            return nullptr;

        const auto itFind = std::find_if(
            vecPartView.begin(),
            vecPartView.end(),
            [partId](const CRenderPartItem& view)
            {
                return view.GetPartId() == partId;
            });

        return itFind != vecPartView.end()
            ? &(*itFind)
            : nullptr;
    }

    const CRenderPartItem* HitTestPart(
        const std::vector<CRenderPartItem>& vecPartView,
        const Vec2& mousePosition)
    {
        const auto itFind = std::find_if(
            vecPartView.crbegin(),
            vecPartView.crend(),
            [&mousePosition](const CRenderPartItem& view)
            {
                return view.IsPtInView(mousePosition);
            });

        return itFind != vecPartView.crend()
            ? &(*itFind)
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
}

CPreviewPanel::CPreviewPanel(
    const Creature::CCreature& creature,
    CCreatureEditor& editor,
    CEditorContext& context)
    : m_editor(editor)
    , m_creature(creature)
    , m_editorContext(context)
{
}

void CPreviewPanel::Draw(
    const CreaturePose& pose,
    CTextureCache& textureCache)
{
    CImGuiWindowScope scope("Preview");

    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 area = ImGui::GetContentRegionAvail();

    const ImVec2 center
    {
        origin.x + area.x * 0.5f,
        origin.y + area.y * 0.5f
    };

    constexpr float scale = 0.25f;

    const auto previewTransform =
        CMatrix3x2::CreateScale({ scale, scale })
        *
        CMatrix3x2::CreateTranslation({
            center.x,
            center.y
            });

    const auto vecPartView = BuildPartViews(pose, previewTransform, textureCache);

    HandleInput(pose, previewTransform, vecPartView);

    const auto drawList = ImGui::GetWindowDrawList();
    for (const auto& part : vecPartView)
    {
        const auto corner = part.GetRect().Corner();
        if (m_editorContext.GetPartId() == part.GetPartId())
        {
            DrawSelectPartFrameRect(drawList, corner, part);
        }
        drawList->AddImageQuad(
            part.GetTexture()->GetShaderResourceView(),
            ImVec2{ corner.topLeft.x,     corner.topLeft.y },
            ImVec2{ corner.topRight.x,    corner.topRight.y },
            ImVec2{ corner.bottomRight.x, corner.bottomRight.y },
            ImVec2{ corner.bottomLeft.x,  corner.bottomLeft.y });
    }
    ImGui::Dummy(area);
}

void CPreviewPanel::HandleInput(
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform,
    const std::vector<CRenderPartItem>& vecPartView)
{
    const auto mousePosition = ImGui::GetMousePos();

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        BeginOperation(pose, previewTransform, vecPartView, { mousePosition.x, mousePosition.y });
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        UpdateOperation(pose, vecPartView, previewTransform);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        EndOperation();
    }
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

void CPreviewPanel::BeginOperation(
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform,
    const std::vector<CRenderPartItem>& vecPartView,
    const Vec2& mousePosition)
{
    switch (m_editorContext.GetEditMode())
    {
    case EditMode::Select:
        BeginSelect(vecPartView, mousePosition);
        break;

    case EditMode::Move:
        BeginMove(vecPartView, mousePosition);
        break;

    case EditMode::Scale:
        BeginScale(vecPartView, mousePosition);
        break;

    case EditMode::Rotate:
        BeginRotate(vecPartView, mousePosition, pose, previewTransform);
        break;

    case EditMode::Pivot:
        // BeginPivot(vecPartView, mousePosition);
        break;
    }
}

void CPreviewPanel::EndOperation()
{
    m_operation = {};
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

    if (m_editorContext.GetEditMode() != EditMode::Scale) return;

    DrawResizeHandle(drawList, corner.topLeft);
    DrawResizeHandle(drawList, corner.topRight);
    DrawResizeHandle(drawList, corner.bottomRight);
    DrawResizeHandle(drawList, corner.bottomLeft);
}

void CPreviewPanel::Select(
    const CRenderPartItem* view)
{
    m_editorContext.SelectPart(
        view
        ? view->GetPartId()
        : INVALID_PART_ID);
    m_operation = {};
}

void CPreviewPanel::BeginSelect(
    const std::vector<CRenderPartItem>& vecPartView,
    const Vec2& mousePosition)
{
    Select(HitTestPart(vecPartView, mousePosition));
}

void CPreviewPanel::BeginScale(
    const std::vector<CRenderPartItem>& vecPartView,
    const Vec2& mousePosition)
{
    const auto selectedView = FindPartView(vecPartView, m_editorContext.GetPartId());

    if (!selectedView)
    {
        const auto hitView = HitTestPart(vecPartView, mousePosition);
        Select(hitView);
        return;
    }

    const auto handle = HitTestResizeHandle(selectedView->GetRect(), mousePosition);
    if (handle == ResizeHandle::None) return;

    m_operation =
    {
        OperationType::Resize,
        selectedView->GetPartId()
    };

    const auto& skeleton = m_creature.GetSkeleton();
    auto part = skeleton.FindPartById(selectedView->GetPartId());
    if (!part) return;

    const auto texture = selectedView->GetTexture();
    if (!texture) return;

    const auto direction = GetHandleDirection(handle);

    const auto width = static_cast<float>(texture->GetWidth());
    const float height = static_cast<float>(texture->GetHeight());

    const Vec2 anchorLocal
    {
        -direction.x * width * 0.5f,
        -direction.y * height * 0.5f
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
}

void CPreviewPanel::BeginRotate(
    const std::vector<CRenderPartItem>& vecPartView,
    const Vec2& mousePosition,
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform)
{
    const auto hitView = HitTestPart(vecPartView, mousePosition);

    if (!hitView)
    {
        m_editorContext.SelectPart(INVALID_PART_ID);
        m_operation = {};
        return;
    }

    m_editorContext.SelectPart(hitView->GetPartId());

    const auto& skeleton = m_creature.GetSkeleton();
    const auto* part = skeleton.FindPartById(hitView->GetPartId());
    if (!part) return;

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
    if (!parentScreen.TryInverse(inverseParentScreen)) return;

    const auto mouse = ImGui::GetMousePos();
    const auto mouseParent = inverseParentScreen.TransformPoint({
        mouse.x,
        mouse.y
    });

    const auto& transform = part->bindTransform;
    const Vec2 pivotParent
    {
        transform.GetPosition().x + part->pivot.x,
        transform.GetPosition().y + part->pivot.y
    };

    const auto angle = CAngle::Normalize(
        std::atan2(mouseParent.y - pivotParent.y, mouseParent.x - pivotParent.x));
 
    m_rotateState =
    {
        transform.GetRotation(),
        angle,
        pivotParent
    };

    m_operation =
    {
        OperationType::Rotate,
        part->id
    };
}

void CPreviewPanel::BeginMove(
    const std::vector<CRenderPartItem>& vecPartView,
    const Vec2& mousePosition)
{
    const auto hitView = HitTestPart(vecPartView, mousePosition);

    if (!hitView)
    {
        Select(nullptr);
        return;
    }

    Select(hitView);

    m_operation =
    {
        OperationType::Move,
        hitView->GetPartId()
    };
}

void CPreviewPanel::UpdateOperation(
    const CreaturePose& pose,
    const std::vector<CRenderPartItem>& vecPartView,
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
    default:
        break;
    }
}

void CPreviewPanel::MovePart(
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform)
{
    const auto& skeleton = m_creature.GetSkeleton();

    auto part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    CMatrix3x2 parentWorld;
    if (part->parentId != INVALID_PART_ID)
    {
        const auto parent = skeleton.FindPartById(part->parentId);
        if (parent) parentWorld = CPartTransformBuilder::BuildWorld(*parent, skeleton, pose);
    }

    const auto parentScreen = parentWorld * previewTransform;

    CMatrix3x2 inverse;
    if (!parentScreen.TryInverse(inverse)) return;

    const auto mouse = ImGui::GetMousePos();
    const auto delta = ImGui::GetIO().MouseDelta;

    const Vec2 currentMouse = inverse.TransformPoint({
        mouse.x,
        mouse.y
    });

    const Vec2 previousMouse = inverse.TransformPoint({
        mouse.x - delta.x,
        mouse.y - delta.y
    });

    auto transform = part->bindTransform;

    transform.GetPosition().x += currentMouse.x - previousMouse.x;
    transform.GetPosition().y += currentMouse.y - previousMouse.y;

    m_editor.SetPartTransform(part->id, transform);
}

void CPreviewPanel::ResizePart(
    const CreaturePose& pose,
    const std::vector<CRenderPartItem>& vecPartView,
    const Creature::Math::CMatrix3x2& previewTransform)
{
    const auto& skeleton = m_creature.GetSkeleton();
    auto part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    const auto* view = FindPartView(vecPartView, m_operation.partId);
    if (!view) return;

    const auto texture = view->GetTexture();
    if (!texture) return;

    CMatrix3x2 parentWorld;

    if (part->parentId != INVALID_PART_ID)
    {
        const auto* parent = skeleton.FindPartById(part->parentId);
        if (parent) parentWorld = CPartTransformBuilder::BuildWorld(*parent, skeleton, pose);
    }

    const auto parentScreen = parentWorld * previewTransform;

    CMatrix3x2 inverseParentScreen;
    if (!parentScreen.TryInverse(inverseParentScreen)) return;

    const ImVec2 mouse =
        ImGui::GetMousePos();

    const Vec2 mouseParent =
        inverseParentScreen.TransformPoint({
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

    const float width = static_cast<float>(texture->GetWidth());
    const float height = static_cast<float>(texture->GetHeight());

    Vec2 newScale
    {
        localDelta.x / (direction.x * width),
        localDelta.y / (direction.y * height)
    };

    constexpr float minScale = 0.05f;

    newScale.x = std::max(newScale.x, minScale);
    newScale.y = std::max(newScale.y, minScale);

    const Vec2 anchorLocal
    {
        -direction.x * width * 0.5f,
        -direction.y * height * 0.5f
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

void CPreviewPanel::RotatePart(
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform)
{
    const auto& skeleton = m_creature.GetSkeleton();

    const auto part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    CMatrix3x2 parentWorld;

    if (part->parentId != INVALID_PART_ID)
    {
        if (const auto parent = skeleton.FindPartById(part->parentId))
        {
            parentWorld = CPartTransformBuilder::BuildWorld(*parent, skeleton, pose);
        }
    }

    const auto parentScreen = parentWorld * previewTransform;

    CMatrix3x2 inverse;
    if (!parentScreen.TryInverse(inverse)) return;

    const auto mouse = ImGui::GetMousePos();
    const auto mouseParent = inverse.TransformPoint({
        mouse.x,
        mouse.y
    });

    const float currentAngle = CAngle::Normalize(
        std::atan2(
            mouseParent.y - m_rotateState.pivot.y,
            mouseParent.x - m_rotateState.pivot.x));

    const float delta = currentAngle - m_rotateState.fMouseAngleStart;

    auto transform = part->bindTransform;
    transform.SetRotation(m_rotateState.fPartRotationStart + delta);

    m_editor.SetPartTransform(part->id, transform);
}

std::vector<CRenderPartItem> CPreviewPanel::BuildPartViews(
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform,
    CTextureCache& textureCache)
{
    const auto& skeleton = m_creature.GetSkeleton();

    std::vector<CRenderPartItem> result;
    result.reserve(skeleton.Parts().size());

    for (const auto& part : skeleton.Parts())
    {
        const auto appearance = m_creature.GetAppearance().FindByPartId(part.id);
        if (!appearance) continue;

        auto texture = textureCache.Load(appearance->texturePath);
        if (!texture) continue;

        const auto worldTransform = CPartTransformBuilder::BuildWorld(part, skeleton, pose);
        const auto screenTransform = worldTransform * previewTransform;

        const Vec2 size
        {
            static_cast<float>(texture->GetWidth()),
            static_cast<float>(texture->GetHeight())
        };

        CTransformRect rect(size, screenTransform);

        result.emplace_back(
            part.id,
            std::move(rect),
            std::move(texture));
    }

    return result;
}
