#include "pch.h"
#include "Skeleton.h"

#include <unordered_set>

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

const std::vector<Part>& CSkeleton::Parts() const noexcept
{
    return m_vecPart;
}

std::vector<Part>& CSkeleton::Parts() noexcept
{
    return m_vecPart;
}

Part* CSkeleton::FindPartByName(std::string_view name)
{
    return FindPart(m_vecPart, [name](const Creature::Part& part) { return std::string_view(part.strName) == name; });
}

const Part* CSkeleton::FindPartByName(const std::string_view name) const
{
    return FindPart(m_vecPart, [name](const Part& part) { return std::string_view(part.strName) == name; });
}

Part* CSkeleton::FindPartById(PartId id)
{
    if (id == INVALID_PART_ID) return nullptr;
    return FindPart(m_vecPart, [id](const Part& part) { return part.id == id; });
}

const Part* CSkeleton::FindPartById(PartId id) const
{
    if (id == INVALID_PART_ID) return nullptr;
    return FindPart(m_vecPart, [id](const Part& part) { return part.id == id; });
}

PartId CSkeleton::AddPart(const std::string_view name, PartId parentId)
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

PartId  CSkeleton::AddPart(Part&& part)
{
    part.id = m_nextPartId++;

    const auto partId = part.id;
    m_vecPart.emplace_back(std::move(part));

    return partId;
}

bool CSkeleton::AddPartWithId(Part&& part)
{
    if (part.id == INVALID_PART_ID) return false;

    if (FindPartById(part.id)) return false;

    m_nextPartId = std::max(m_nextPartId, part.id + 1);

    m_vecPart.emplace_back(std::move(part));

    return true;
}

void CSkeleton::CollectDescendants(PartId parentId, std::unordered_set<PartId>& ids) const
{
    for (const auto& part : m_vecPart)
    {
        if (part.parentId == INVALID_PART_ID || part.parentId != parentId)
        {
            continue;
        }

        ids.insert(part.id);

        CollectDescendants(part.id, ids);
    }
}

bool CSkeleton::RemovePart(PartId id)
{
    if (FindPartIndexById(id) == INVALID_PART_INDEX) return false;

    std::unordered_set<PartId> removeIds;
    removeIds.insert(id);

    CollectDescendants(id, removeIds);

    m_vecPart.erase(
        std::remove_if(
            m_vecPart.begin(),
            m_vecPart.end(),
            [&removeIds](const Part& part)
            {
                return removeIds.find(part.id) != removeIds.cend();
            }),
        m_vecPart.end());

    return true;
}

PartId CSkeleton::FindPartIdByName(const std::string_view name) const
{
    const auto findPart = FindPartByName(name);
    return findPart ? findPart->id : INVALID_PART_ID;
}

int CSkeleton::FindPartIndexById(PartId id) const
{
    const auto itFind = std::find_if(m_vecPart.cbegin(), m_vecPart.cend(), [id](const Part& part)
        {
            return part.id == id;
        });
    if (itFind == m_vecPart.cend()) return INVALID_PART_INDEX;
    return static_cast<int>(std::distance(m_vecPart.begin(), itFind));
}
