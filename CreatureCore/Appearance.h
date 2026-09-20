#pragma once

#include "PartAppearance.h"
#include "PartId.h"

#include <filesystem>
#include <vector>

namespace Creature
{
    class CAppearance
    {
    public:
        void SetTexture(PartId partId, const std::filesystem::path& path);

        const PartAppearance* FindByPartId(PartId partId) const;

    private:
        std::vector<PartAppearance> m_vecPart;
    };
}
