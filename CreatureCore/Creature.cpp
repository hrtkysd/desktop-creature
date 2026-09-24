#include "pch.h"
#include "Animation.h"
#include "Appearance.h"
#include "Creature.h"
#include "Genome.h"
#include "Skeleton.h"

#include <string>
#include <utility>

using namespace Creature;
using namespace Creature::Animation;

struct CCreature::Impl
{
    Genome genome{};
    CAppearance appearance{};
    CSkeleton skeleton;

    AnimationId nextAnimationId = MIN_ANIMATION_ID;
    std::vector<CAnimation> vecAnimation;

    std::string strName;
};

CCreature::CCreature()
    : m_impl(std::make_unique<Impl>())
{}

CCreature::~CCreature() = default;

CCreature::CCreature(CCreature&&) noexcept = default;

CCreature& CCreature::operator=(CCreature&& rhs) noexcept
{
    if (this == &rhs) return *this;

    *m_impl = std::move(*rhs.m_impl);
    return *this;
}

const Genome& CCreature::GetGenome() const
{
    return m_impl->genome;
}

Genome& CCreature::GetGenome()
{
    return m_impl->genome;
}

const CSkeleton& CCreature::GetReadonlySkeleton() const
{
    return m_impl->skeleton;
}

CSkeleton& CCreature::GetSkeleton()
{
    return m_impl->skeleton;
}

const CAppearance& CCreature::GetReadonlyAppearance() const
{
    return m_impl->appearance;
}

CAppearance& CCreature::GetAppearance()
{
    return m_impl->appearance;
}

std::vector<CAnimation>& Creature::CCreature::GetAnimations()
{
    return m_impl->vecAnimation;
}

const std::string& CCreature::GetName() const
{
    return m_impl->strName;
}

void CCreature::SetName(const std::string& strName)
{
    m_impl->strName = strName;
}

const std::vector<Animation::CAnimation>& Creature::CCreature::GetReadonlyAnimations() const
{
    return m_impl->vecAnimation;
}

AnimationId CCreature::AddAnimation(CAnimation&& animation)
{
    const auto id = GenerateAnimationId();
    if (id == INVALID_ANIMATION_ID) return INVALID_ANIMATION_ID;
    animation.SetAnimationId(id);
    m_impl->vecAnimation.emplace_back(std::move(animation));
    return id;
}

AnimationId CCreature::AddAnimationWithId(AnimationId id, const std::string& name)
{
    if (id == INVALID_ANIMATION_ID) return INVALID_ANIMATION_ID;
    if (id == std::numeric_limits<AnimationId>::max()) return INVALID_ANIMATION_ID;
    if (FindAnimationById(id) != nullptr) return INVALID_ANIMATION_ID;

    m_impl->vecAnimation.emplace_back(CAnimation::NewAnimation(id, name));

    UpdateNextAnimationId(id);

    return id;
}

AnimationId CCreature::GenerateAnimationId()
{
    const auto id = m_impl->nextAnimationId;

    if (id == INVALID_ANIMATION_ID)
    {
        return INVALID_ANIMATION_ID;
    }

    if (id == std::numeric_limits<AnimationId>::max())
    {
        m_impl->nextAnimationId = INVALID_ANIMATION_ID;
    }
    else
    {
        ++m_impl->nextAnimationId;
    }

    return id;
}

void CCreature::UpdateNextAnimationId(AnimationId id)
{
    m_impl->nextAnimationId = std::max(m_impl->nextAnimationId, id + 1);
}

bool CCreature::RemoveAnimation(AnimationId id)
{
    auto& vecAnimation = m_impl->vecAnimation;

    const auto it = std::find_if(vecAnimation.begin(), vecAnimation.end(), [id](const CAnimation& animation)
        {
            return id == animation.GetAnimationId();
        });

    if (it == vecAnimation.end()) return false;

    vecAnimation.erase(it);
    return true;
}

AnimationId CCreature::AddNewAnimation(const std::string& strName)
{
    const auto id = GenerateAnimationId();
    if (id == INVALID_ANIMATION_ID)
    {
        return INVALID_ANIMATION_ID;
    }

    m_impl->vecAnimation.emplace_back(CAnimation::NewAnimation(id, strName));
    return id;
}

CAnimation* CCreature::FindAnimationById(AnimationId id)
{
    return const_cast<CAnimation*>(std::as_const(*this).FindReadonlyAnimationById(id));
}

const CAnimation* CCreature::FindReadonlyAnimationById(AnimationId id) const
{
    if (id == INVALID_ANIMATION_ID) return nullptr;

    auto& vecAnimation = m_impl->vecAnimation;
    auto itFind = std::find_if(vecAnimation.begin(), vecAnimation.end(), [id](const CAnimation& animation)
        {
            return animation.GetAnimationId() == id;
        });
    return itFind != vecAnimation.cend()
        ? &(*itFind)
        : nullptr;
}
