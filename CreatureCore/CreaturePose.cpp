#include "pch.h"
#include "CreaturePose.h"
#include "Skeleton.h"
#include "Transform2D.h"

using namespace Creature;
using namespace Creature::Math;

CCreaturePose CCreaturePose::Default(const CSkeleton& skeleton)
{
    CCreaturePose pose;

    auto& vecTransform = pose.GetPartTransform();
    vecTransform.resize(skeleton.Parts().size());

    return pose;
}

std::vector<CTransform2D>& CCreaturePose::GetPartTransform()
{
    return m_vecPartTransform;
}

const std::vector<CTransform2D>& CCreaturePose::GetPartTransform() const
{
    return m_vecPartTransform;
}

CTransform2D& CCreaturePose::GetRootTransform()
{
    return m_rootTransform;
}

const CTransform2D& CCreaturePose::GetRootTransform() const
{
    return m_rootTransform;
}

void CCreaturePose::SetRootTransform(const CTransform2D& transform)
{
    m_rootTransform = transform;
}

Vec2& CCreaturePose::GetEyeOffset()
{
    return m_eyeOffset;
}

const Vec2& CCreaturePose::GetEyeOffset() const
{
    return m_eyeOffset;
}

void CCreaturePose::SetEyeOffset(const Vec2& offset)
{
    m_eyeOffset = offset;
}

bool Creature::CCreaturePose::IsBlink() const
{
    return m_isBlink;
}

void CCreaturePose::SetBlink(bool isBlink)
{
    m_isBlink = isBlink;
}
