#include "pch.h"
#include "Animation.h"
#include "Appearance.h"
#include "Creature.h"
#include "CreaturePose.h"
#include "ImGuiWindowScope.h"
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
    CTransform2D BuildAnimatedTransform(
        const Part& part,
        const CTransform2D& poseTransform)
    {
        auto transform = part.bindTransform;

        transform.GetPosition().x +=
            poseTransform.GetPosition().x;

        transform.GetPosition().y +=
            poseTransform.GetPosition().y;

        transform.SetRotation(
            transform.GetRotation() +
            poseTransform.GetRotation());

        transform.GetScale().x *=
            poseTransform.GetScale().x;

        transform.GetScale().y *=
            poseTransform.GetScale().y;

        return transform;
    }
    Vec2 GetHandleDirection(ResizeHandle handle)
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

    const CRenderPartItem* FindPartView(const std::vector<CRenderPartItem>& vecPartView, PartId partId)
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
        const auto corners = rect.Corners();

        if (IsInResizeHandle(point, corners.topLeft)) return ResizeHandle::TopLeft;
        if (IsInResizeHandle(point, corners.topRight)) return ResizeHandle::TopRight;
        if (IsInResizeHandle(point, corners.bottomRight)) return ResizeHandle::BottomRight;
        if (IsInResizeHandle(point, corners.bottomLeft)) return ResizeHandle::BottomLeft;

        return ResizeHandle::None;
    }

    CMatrix3x2 BuildLocalTransform(const CTransform2D transform, const Vec2& pivot)
    {
        return
            CMatrix3x2::CreateTranslation({ -pivot.x, -pivot.y }) *
            CMatrix3x2::CreateScale(transform.GetScale()) *
            CMatrix3x2::CreateRotation(transform.GetRotation()) *
            CMatrix3x2::CreateTranslation(pivot) *
            CMatrix3x2::CreateTranslation(transform.GetPosition());
    }

    CMatrix3x2 BuildWorldTransform(
        const Part& part,
        const CSkeleton& skeleton,
        const CreaturePose& pose)
    {
        const auto index = skeleton.FindPartIndexById(part.id);

        if (index == Creature::INVALID_PART_INDEX) return part.bindTransform.ToMatrix();

        const auto transform = BuildAnimatedTransform(part, pose.vecPartTransform.at(index));
        const auto local = BuildLocalTransform(transform, part.pivot);

        if (part.parentId == INVALID_PART_ID) return local;

        const auto parent = skeleton.FindPartById(part.parentId);
        if (!parent) return local;

        return
            local
            *
            BuildWorldTransform(
                *parent,
                skeleton,
                pose);
    }
}

void CPreviewPanel::Draw(CCreature& creature, const CreaturePose& pose, CTextureCache& textureCache)
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

    m_fAnimationTime += ImGui::GetIO().DeltaTime;

    const auto vecPartView = BuildPartViews(creature, pose, previewTransform, textureCache);

    HandleInput(creature, pose, previewTransform, vecPartView);

    const auto drawList = ImGui::GetWindowDrawList();
    for (const auto& part : vecPartView)
    {
        const auto corners = part.GetRect().Corners();
        if (m_selectedPartId == part.GetPartId())
        {
            const ImU32 color = IM_COL32(255, 0, 0, 255);
            drawList->AddLine(
                ImVec2{ corners.topLeft.x, corners.topLeft.y },
                ImVec2{ corners.topRight.x, corners.topRight.y },
                color,
                2.0f);
            drawList->AddLine(
                ImVec2{ corners.topRight.x, corners.topRight.y },
                ImVec2{ corners.bottomRight.x, corners.bottomRight.y },
                color,
                2.0f);
            drawList->AddLine(
                ImVec2{ corners.bottomRight.x, corners.bottomRight.y },
                ImVec2{ corners.bottomLeft.x, corners.bottomLeft.y },
                color,
                2.0f);
            drawList->AddLine(
                ImVec2{ corners.bottomLeft.x, corners.bottomLeft.y },
                ImVec2{ corners.topLeft.x, corners.topLeft.y },
                color,
                2.0f);

            DrawResizeHandle(drawList, corners.topLeft);
            DrawResizeHandle(drawList, corners.topRight);
            DrawResizeHandle(drawList, corners.bottomRight);
            DrawResizeHandle(drawList, corners.bottomLeft);
        }
    
        drawList->AddImageQuad(
            part.GetTexture()->GetShaderResourceView(),
            ImVec2{ corners.topLeft.x,     corners.topLeft.y },
            ImVec2{ corners.topRight.x,    corners.topRight.y },
            ImVec2{ corners.bottomRight.x, corners.bottomRight.y },
            ImVec2{ corners.bottomLeft.x,  corners.bottomLeft.y });
    }

    ImGui::Dummy(area);
}

void CPreviewPanel::HandleInput(
    CCreature& creature,
    const CreaturePose& pose,
    const CMatrix3x2& previewTransform,
    const std::vector<CRenderPartItem>& vecPartView)
{
    const auto mousePosition = ImGui::GetMousePos();

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        BeginOpeartion(creature, vecPartView, { mousePosition.x, mousePosition.y });
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        UpdateOperation(creature, pose, vecPartView, previewTransform);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        EndOperation();
    }
}

void CPreviewPanel::DrawResizeHandle(ImDrawList* drawList, const Vec2& position)
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

void CPreviewPanel::BeginOpeartion(CCreature& creature, const std::vector<CRenderPartItem>& vecPartView, const Vec2& mousePosition)
{
    const auto* selectedView = FindPartView(vecPartView, m_selectedPartId);

    if (selectedView && TryBeginResize(creature, *selectedView, mousePosition)) return;
    
    const auto hitView = HitTestPart(vecPartView, mousePosition);
    if (!hitView)
    {
        m_selectedPartId = INVALID_PART_ID;
        m_operation = {};
        return;
    }

    m_selectedPartId = hitView->GetPartId();

    m_operation =
    {
        OperationType::Move,
        m_selectedPartId
    };
}

void CPreviewPanel::EndOperation()
{
    m_operation = {};
}

bool CPreviewPanel::TryBeginResize(CCreature& creature, const CRenderPartItem& view, const Vec2& mousePosition)
{
    const auto handle = HitTestResizeHandle(view.GetRect(), mousePosition);

    if (handle == ResizeHandle::None) return false;

    m_operation =
    {
        OperationType::Resize,
        view.GetPartId()
    };

    auto& skeleton = creature.GetSkeleton();
    auto part = skeleton.FindPartById(view.GetPartId());

    if (!part) return false;

    const auto texture = view.GetTexture();
    if (!texture) return false;

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

    return true;
}

void CPreviewPanel::UpdateOperation(CCreature& creature, const CreaturePose& pose, const std::vector<CRenderPartItem>& vecPartView, const CMatrix3x2& previewTransform)
{
    switch (m_operation.eType)
    {
    case OperationType::Move:
        MovePart(creature, pose, previewTransform);
        break;
    case OperationType::Resize:
        ResizePart(creature, pose, vecPartView, previewTransform);
        break;
    default:
        break;
    }
}

void CPreviewPanel::MovePart(CCreature& creature, const CreaturePose& pose, const CMatrix3x2& previewTransform)
{
    auto& skeleton = creature.GetSkeleton();

    auto* part = skeleton.FindPartById(m_operation.partId);
    if (!part) return;

    CMatrix3x2 parentWorld;
    if (part->parentId != INVALID_PART_ID)
    {
        const auto parent = skeleton.FindPartById(part->parentId);
        if (parent) parentWorld = BuildWorldTransform(*parent, skeleton, pose);
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

    auto& position = part->bindTransform.GetPosition();

    position.x += currentMouse.x - previousMouse.x;
    position.y += currentMouse.y - previousMouse.y;
}

void CPreviewPanel::ResizePart(Creature::CCreature& creature, const CreaturePose& pose, const std::vector<CRenderPartItem>& vecPartView, const Creature::Math::CMatrix3x2& previewTransform)
{
    auto& skeleton = creature.GetSkeleton();
    auto* part = skeleton.FindPartById(m_operation.partId);

    if (!part) return;

    const auto* view = FindPartView(vecPartView, m_operation.partId);

    if (!view) return;

    const auto texture = view->GetTexture();

    if (!texture) return;

    CMatrix3x2 parentWorld;

    if (part->parentId != INVALID_PART_ID)
    {
        const auto* parent = skeleton.FindPartById(part->parentId);
        if (parent) parentWorld = BuildWorldTransform(*parent, skeleton, pose);
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

    const auto inverseRotation = CMatrix3x2::CreateRotation(-part->bindTransform.GetRotation());

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
        CMatrix3x2::CreateRotation(part->bindTransform.GetRotation());

    const auto anchorOffset = scaleRotation.TransformPoint(anchorLocal);

    Vec2 newPosition
    {
        m_resizeState.anchor.x - anchorOffset.x,
        m_resizeState.anchor.y - anchorOffset.y
    };

    part->bindTransform.GetScale() = newScale;
    part->bindTransform.GetPosition() = newPosition;
}

std::vector<CRenderPartItem> CPreviewPanel::BuildPartViews(CCreature& creature, const CreaturePose& pose, const CMatrix3x2& previewTransform, CTextureCache& textureCache)
{
    const auto& skeleton = creature.GetSkeleton();

    std::vector<CRenderPartItem> result;
    result.reserve(skeleton.Parts().size());

    for (const auto& part : skeleton.Parts())
    {
        const auto appearance = creature.GetAppearance().FindByPartId(part.id);
        if (!appearance) continue;

        auto texture = textureCache.Load(appearance->texturePath);
        if (!texture) continue;

        const auto worldTransform = BuildWorldTransform(part, skeleton, pose);
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
