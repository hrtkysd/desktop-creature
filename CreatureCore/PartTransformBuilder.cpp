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

CMatrix3x2 CPartTransformBuilder::BuildWorld(const Part& part, const CSkeleton& skeleton, const CreaturePose& pose)
{
    auto transform = part.bindTransform;

    const auto index = skeleton.FindPartIndexById(part.id);

    if (index != INVALID_PART_INDEX)
    {
        transform = BuildAnimatedTransform(part, pose.vecPartTransform.at(index));
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
