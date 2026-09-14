#include "pch.h"
#include "Skelton.h"

using namespace Creature;

const std::vector<Part>& Creature::CSkeleton::Parts() const noexcept
{
    return m_vecPart;
}

std::vector<Part>& Creature::CSkeleton::Parts() noexcept
{
    return m_vecPart;
}

int Creature::CSkeleton::FindPartIndex(const std::string_view& name) const
{
    const auto itFound = std::find_if(m_vecPart.cbegin(), m_vecPart.cend(), [&name](const Creature::Part& part)
        { return std::string_view(part.strName) == name; });
    if (itFound == m_vecPart.cend()) return -1;
    return static_cast<int>(std::distance(m_vecPart.cbegin(), itFound));
}

