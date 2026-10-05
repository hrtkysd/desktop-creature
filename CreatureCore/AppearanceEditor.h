#pragma once

#include "PartId.h"

#include <filesystem>
#include <vector>

namespace Creature
{
    class CAppearance;

    struct PartAppearance;

    namespace IO
    {
        class CCreatureIO;
    }

    namespace Editor
    {
        class CAppearanceEditor
        {
            friend class Creature::IO::CCreatureIO;
        public:
            CAppearanceEditor() = delete;
            explicit CAppearanceEditor(Creature::CAppearance& appearance);
        public:
            const Creature::CAppearance& GetAppearance() const;

            void AddPart(Creature::PartId partId, const std::filesystem::path& path);
            bool RemovePart(Creature::PartId partId);

            bool MoveForward(Creature::PartId partId);
            bool MoveBackward(Creature::PartId partId);
        private:
            void SwapAppearance(std::vector<Creature::PartAppearance>&& vecPartAppearance);
        private:
            Creature::CAppearance& m_appearance;
        };
    }
}
