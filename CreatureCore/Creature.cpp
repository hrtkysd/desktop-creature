#include "pch.h"
#include "Appearance.h"
#include "Creature.h"
#include "Genome.h"
#include "Skeleton.h"

using namespace Creature;

struct CCreature::Impl
{
    Genome genome{};
    Appearance appearance{};
    CSkeleton skelton;
};

CCreature::CCreature()
    : m_impl(std::make_unique<Impl>())
{}

CCreature::~CCreature() = default;

CCreature::CCreature(CCreature&&) noexcept = default;
CCreature& CCreature::operator=(CCreature&&) noexcept = default;

const Genome& CCreature::GetGenome() const
{
    return m_impl->genome;
}

Genome& CCreature::GetGenome()
{
    return m_impl->genome;
}

const CSkeleton& CCreature::GetSkeleton() const
{
    return m_impl->skelton;
}

CSkeleton& CCreature::GetSkeleton()
{
    return m_impl->skelton;
}

const Appearance& CCreature::GetAppearance() const
{
    return m_impl->appearance;
}

Appearance& CCreature::GetAppearance()
{
    return m_impl->appearance;
}
