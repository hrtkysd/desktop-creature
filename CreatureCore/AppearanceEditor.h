#pragma once

#include "PartId.h"

#include <filesystem>

namespace Creature
{
    class CAppearance;

    namespace Editor
    {
        class CAppearanceEditor
        {
        public:
            CAppearanceEditor() = delete;
            explicit CAppearanceEditor(Creature::CAppearance& apperance);
        public:
            const Creature::CAppearance& GetAppearance() const;
            void SetTexture(Creature::PartId partId, const std::filesystem::path& path);
            bool RemovePart(Creature::PartId partId);
        private:
            Creature::CAppearance& m_appearance;
        };
    }
}
