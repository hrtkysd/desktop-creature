#include "pch.h"
#include "Skeleton.h"

using namespace Creature;

namespace
{
    template <typename Predicate>
    Creature::Part* FindPart(
        std::vector<Creature::Part>& parts,
        Predicate predicate)
    {
        const auto it = std::find_if(
            parts.begin(),
            parts.end(),
            predicate);

        return it != parts.end()
            ? &(*it)
            : nullptr;
    }

    template <typename Predicate>
    const Creature::Part* FindPart(
        const std::vector<Creature::Part>& parts,
        Predicate predicate)
    {
        const auto it = std::find_if(
            parts.cbegin(),
            parts.cend(),
            predicate);

        return it != parts.cend()
            ? &(*it)
            : nullptr;
    }
}

const std::vector<Part>& Creature::CSkeleton::Parts() const noexcept
{
    return m_vecPart;
}

std::vector<Part>& Creature::CSkeleton::Parts() noexcept
{
    return m_vecPart;
}

Part* Creature::CSkeleton::FindPartByName(std::string_view name)
{
    return FindPart(m_vecPart, [name](const Creature::Part& part) { return std::string_view(part.strName) == name; });
}

const Creature::Part* Creature::CSkeleton::FindPartByName(const std::string_view name) const
{
    return FindPart(m_vecPart, [name](const Creature::Part& part){ return std::string_view(part.strName) == name; });
}

Part* Creature::CSkeleton::FindPartById(PartId id)
{
    if (id == INVALID_PART_ID) return nullptr;
    return FindPart(m_vecPart, [id](const Creature::Part& part) { return part.id == id; });
}

const Creature::Part* Creature::CSkeleton::FindPartById(PartId id) const
{
    if (id == INVALID_PART_ID) return nullptr;
    return FindPart(m_vecPart, [id](const Creature::Part& part) { return part.id == id; });
}

PartId Creature::CSkeleton::AddPart(const std::string_view name, PartId parentId)
{
    if (parentId != INVALID_PART_ID && !FindPartById(parentId))
    {
        return INVALID_PART_ID;
    }

    Part part{};
    part.id = m_nextPartId++;
    part.strName = name;
    part.parentId = parentId;

    m_vecPart.emplace_back(std::move(part));

    return m_vecPart.back().id;
}

PartId  Creature::CSkeleton::AddPart(Part&& part)
{
    part.id = m_nextPartId++;

    const auto partId = part.id;
    m_vecPart.emplace_back(std::move(part));

    return partId;
}

bool Creature::CSkeleton::AddPartWithId(Part&& part)
{
    if (part.id == INVALID_PART_ID) return false;

    if (FindPartById(part.id)) return false;

    m_nextPartId = std::max(m_nextPartId, part.id + 1);

    m_vecPart.emplace_back(std::move(part));

    return true;
}

PartId Creature::CSkeleton::FindPartIdByName(const std::string_view name) const
{
    const auto findPart = FindPartByName(name);
    return findPart ? findPart->id : INVALID_PART_ID;
}
