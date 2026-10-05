#include "pch.h"
#include "Appearance.h"
#include "PartAppearance.h"

using namespace Creature;

CAppearance::CAppearance() = default;

CAppearance::CAppearance(std::vector<PartAppearance>&& vecPart)
    : m_vecPart(std::move(vecPart))
{
    std::stable_sort(m_vecPart.begin(), m_vecPart.end(),
        [](const PartAppearance& lhs, const PartAppearance& rhs)
        {
            return lhs.zOrder < rhs.zOrder;
        });
}

const std::vector<PartAppearance>& CAppearance::Parts() const
{
    return m_vecPart;
}

ZOrder CAppearance::NextZOrder()
{
    if (m_vecPart.empty()) return 0;
    return m_vecPart.back().zOrder + 1;
}

bool CAppearance::MoveForward(PartId partId)
{
    const auto it = std::find_if(m_vecPart.begin(), m_vecPart.end(),
        [partId](const PartAppearance& appearance)
        {
            return appearance.partId == partId;
        });

    if (it == m_vecPart.end()) return false;

    const auto itNext = std::next(it);
    if (itNext == m_vecPart.end()) return false;

    std::swap(it->zOrder, itNext->zOrder);
    std::iter_swap(it, itNext);

    return true;
}

bool CAppearance::MoveBackward(PartId partId)
{
    const auto it = std::find_if(m_vecPart.begin(), m_vecPart.end(),
        [partId](const PartAppearance& appearance)
        {
            return appearance.partId == partId;
        });

    if (it == m_vecPart.end() || it == m_vecPart.begin()) return false;

    const auto itPrev = std::prev(it);

    std::swap(it->zOrder, itPrev->zOrder);
    std::iter_swap(it, itPrev);

    return true;
}

void CAppearance::AddPart(PartId partId, const std::filesystem::path& path)
{
    const auto itFind = std::find_if(m_vecPart.begin(), m_vecPart.end(), [partId](const PartAppearance& appearance)
        {
            return appearance.partId == partId;
        });

    if (itFind != m_vecPart.cend())
    {
        itFind->texturePath = path;
        return;
    }

    m_vecPart.emplace_back(PartAppearance{ partId, path, NextZOrder() });
}

bool CAppearance::RemovePart(PartId partId)
{
    const auto it = std::remove_if(m_vecPart.begin(), m_vecPart.end(), [partId](const PartAppearance& appearance)
        {
            return appearance.partId == partId;
        });

    if (it == m_vecPart.cend()) return false;

    m_vecPart.erase(it);
    return true;
}

const PartAppearance* CAppearance::FindByPartId(PartId partId) const
{
    const auto itFind = std::find_if(m_vecPart.cbegin(), m_vecPart.cend(), [partId](const PartAppearance& appearance)
        {
            return appearance.partId == partId;
        });
    if (itFind == m_vecPart.cend()) return nullptr;
    return std::addressof(*itFind);
}


bool CAppearance::CanMoveBackward(PartId partId) const
{
    const auto it = std::find_if(m_vecPart.cbegin(), m_vecPart.cend(),
        [partId](const PartAppearance& appearance)
        {
            return appearance.partId == partId;
        });

    return it != m_vecPart.cend() && it != m_vecPart.cbegin();
}

bool CAppearance::CanMoveForward(PartId partId) const
{
    const auto it = std::find_if(m_vecPart.cbegin(), m_vecPart.cend(),
        [partId](const PartAppearance& appearance)
        {
            return appearance.partId == partId;
        });

    return it != m_vecPart.cend() && std::next(it) != m_vecPart.cend();
}
