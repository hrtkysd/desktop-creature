#include "pch.h"
#include "Creature.h"
#include "Genome.h"
#include "Motion.h"

using namespace Creature;

struct CCreature::Impl
{
    Genome genome{};
    std::vector<CMotion> vecMotion;
    MotionId nextMotionId = MIN_MOTION_ID;

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

    m_impl = std::move(rhs.m_impl);
    return *this;
}

CCreature CCreature::Clone() const
{
    CCreature creature;
    creature.m_impl->strName = m_impl->strName;
    creature.m_impl->vecMotion.reserve(m_impl->vecMotion.size());
    for (const auto& motion : m_impl->vecMotion)
    {
        creature.m_impl->vecMotion.push_back(motion.Clone());
    }
    return creature;
}

const Genome& CCreature::GetGenome() const
{
    return m_impl->genome;
}

Genome& CCreature::MutableGenome()
{
    return m_impl->genome;
}

std::vector<CMotion>& CCreature::MutableMotions()
{
    return m_impl->vecMotion;
}

CMotion* CCreature::FindMutableMotionById(MotionId id)
{
    return const_cast<CMotion*>(std::as_const(*this).FindMotionById(id));
}

MotionId CCreature::AddMotion(CMotion&& motion)
{
    const auto id = GenerateMotionId();
    if (id == INVALID_MOTION_ID) return INVALID_MOTION_ID;
    motion.SetMotionId(id);
    m_impl->vecMotion.emplace_back(std::move(motion));
    return id;
}

bool CCreature::RemoveMotion(MotionId id)
{
    auto& vecMotion = m_impl->vecMotion;

    const auto it = std::find_if(vecMotion.begin(), vecMotion.end(), [id](const CMotion& motion)
        {
            return id == motion.GetMotionId();
        });

    if (it == vecMotion.end()) return false;

    vecMotion.erase(it);
    return true;
}

MotionId CCreature::AddNewMotion(const std::string& strName)
{
    const auto id = GenerateMotionId();
    if (id == INVALID_MOTION_ID)
    {
        return INVALID_MOTION_ID;
    }

    m_impl->vecMotion.emplace_back(CMotion::NewMotion(id, strName));
    return id;
}

MotionId CCreature::AddMotionWithId(MotionId id, const std::string& strName)
{
    if (id == INVALID_MOTION_ID) return INVALID_MOTION_ID;
    if (id == std::numeric_limits<MotionId>::max()) return INVALID_MOTION_ID;
    if (FindMotionById(id) != nullptr) return INVALID_MOTION_ID;

    m_impl->vecMotion.emplace_back(CMotion::NewMotion(id, strName));

    UpdateNextMotionId(id);

    return id;
}

const std::string& CCreature::GetName() const
{
    return m_impl->strName;
}

void CCreature::SetName(const std::string& strName)
{
    m_impl->strName = strName;
}

const std::vector<CMotion>& CCreature::GetMotions() const
{
    return m_impl->vecMotion;
}

const CMotion* CCreature::FindMotionById(MotionId id) const
{
    if (id == INVALID_MOTION_ID) return nullptr;

    const auto& vecMotion = m_impl->vecMotion;
    auto itFind = std::find_if(vecMotion.begin(), vecMotion.end(), [id](const CMotion& motion)
        {
            return motion.GetMotionId() == id;
        });
    return itFind != vecMotion.cend()
        ? std::addressof(*itFind)
        : nullptr;
}

MotionId CCreature::GenerateMotionId()
{
    const auto id = m_impl->nextMotionId;

    if (id == INVALID_MOTION_ID)
    {
        return INVALID_MOTION_ID;
    }

    if (id == std::numeric_limits<MotionId>::max())
    {
        m_impl->nextMotionId = INVALID_MOTION_ID;
    }
    else
    {
        ++m_impl->nextMotionId;
    }

    return id;
}

void CCreature::UpdateNextMotionId(MotionId id)
{
    m_impl->nextMotionId = std::max(m_impl->nextMotionId, id + 1);
}
