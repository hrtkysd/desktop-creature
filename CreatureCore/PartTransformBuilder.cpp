#include "pch.h"
#include "CreaturePose.h"
#include "Matrix3x2.h"
#include "Part.h"
#include "PartTransformBuilder.h"
#include "Skeleton.h"
#include "Transform2D.h"

using namespace Creature;
using namespace Creature::Math;

namespace
{
    CTransform2D BuildAnimatedTransform(
        const Creature::Part& part,
        const CTransform2D& poseTransform)
    {
        auto transform = part.bindTransform;

        const auto currentPosition = transform.GetPosition();
        transform.SetPosition(
            {
                currentPosition.x + poseTransform.GetPosition().x,
                currentPosition.y + poseTransform.GetPosition().y
            });

        transform.SetRotation(
            transform.GetRotation() +
            poseTransform.GetRotation());

        const auto& currentScale = transform.GetScale();
        transform.SetScale(
            {
                currentScale.x * poseTransform.GetScale().x,
                currentScale.y * poseTransform.GetScale().y
            });

        return transform;
    }
}

CMatrix3x2 CPartTransformBuilder::BuildLocal(const Part& part, const Math::CTransform2D& transform)
{
    const auto& pivot = part.pivot;
    return
        CMatrix3x2::CreateTranslation({ -pivot.x, -pivot.y }) *
        CMatrix3x2::CreateScale(transform.GetScale()) *
        CMatrix3x2::CreateRotation(transform.GetRotation()) *
        CMatrix3x2::CreateTranslation(pivot) *
        CMatrix3x2::CreateTranslation(transform.GetPosition());
}

CMatrix3x2 Creature::Math::CPartTransformBuilder::BuildWorld(
    const Part& part,
    const CSkeleton& skeleton)
{
    const auto local = part.bindTransform.ToMatrix();

    if (part.parentId == INVALID_PART_ID) return local;

    const auto* parent = skeleton.FindPartById(part.parentId);
    if (!parent) return local;
    return
        local
        *
        BuildWorld(*parent, skeleton);
}

CMatrix3x2 CPartTransformBuilder::BuildWorld(
    const Part& part,
    const CSkeleton& skeleton,
    const CCreaturePose& pose)
{
    auto transform = part.bindTransform;

    const auto index = skeleton.FindPartIndexById(part.id);
    const auto& vecPartTransform = pose.GetPartTransform();

    if (index != INVALID_PART_INDEX)
    {
        transform = BuildAnimatedTransform(part, vecPartTransform.at(index));
    }

    const auto local = BuildLocal(part, transform);
    if (part.parentId == INVALID_PART_ID) return local;

    const auto parent = skeleton.FindPartById(part.parentId);
    if (!parent) return local;

    return
        local
        *
        BuildWorld(
            *parent,
            skeleton,
            pose);
}
