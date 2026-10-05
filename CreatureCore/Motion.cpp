#include "pch.h"
#include "Animation.h"
#include "Appearance.h"
#include "Motion.h"
#include "Skeleton.h"

using namespace Creature;
using namespace Creature::Animation;

struct CMotion::MotionImpl
{
    MotionId motionId = INVALID_MOTION_ID;
    CAppearance appearance;
    CSkeleton skeleton;
    CAnimation animation;

    std::string strName;
};

CMotion::CMotion(CMotion&&) noexcept = default;
CMotion& CMotion::operator=(CMotion&&) noexcept = default;

CMotion::CMotion(std::unique_ptr<MotionImpl> impl) noexcept
    : m_impl(std::move(impl))
{
}

CMotion::CMotion(MotionId id, const std::string& strName)
    : m_impl(std::make_unique<MotionImpl>())
{
    m_impl->motionId = id;
    m_impl->strName = strName;
}

CMotion::~CMotion() = default;

const CSkeleton& CMotion::GetSkeleton() const
{
    return m_impl->skeleton;
}

const CAppearance& CMotion::GetAppearance() const
{
    return m_impl->appearance;
}

const CAnimation& CMotion::GetAnimation() const
{
    return m_impl->animation;
}

const std::string& CMotion::GetName() const
{
    return m_impl->strName;
}

MotionId CMotion::GetMotionId() const
{
    return m_impl->motionId;
}

CMotion CMotion::Clone() const
{
    return CMotion(std::make_unique<MotionImpl>(*m_impl));
}

CSkeleton& CMotion::MutableSkeleton()
{
    return m_impl->skeleton;
}

CAppearance& CMotion::MutableAppearance()
{
    return m_impl->appearance;
}

CAnimation& CMotion::MutableAnimation()
{
    return m_impl->animation;
}

CMotion CMotion::NewMotion(
    MotionId id,
    const std::string& strName)
{
    return CMotion(id, strName);
}

void CMotion::SetName(const std::string& strName)
{
    m_impl->strName = strName;
}

void CMotion::SetMotionId(MotionId id)
{
    m_impl->motionId = id;
}
