#pragma once

#include "PartAppearance.h"
#include "PartId.h"

#include <filesystem>
#include <vector>

namespace Creature
{
    namespace Editor
    {
        class CAppearanceEditor;
    }

    class CAppearance
    {
        friend class Editor::CAppearanceEditor;
    public:
        const PartAppearance* FindByPartId(PartId partId) const;
    private:
        void SetTexture(PartId partId, const std::filesystem::path& path);
        bool RemovePart(PartId partId);
    private:
        std::vector<PartAppearance> m_vecPart;
    };
}
