#include "pch.h"
#include "Appearance.h"
#include "PartAppearance.h"

#include <memory>

using namespace Creature;

void CAppearance::SetTexture(PartId partId, const std::filesystem::path& path)
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

    m_vecPart.emplace_back(PartAppearance{ partId, path });
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
