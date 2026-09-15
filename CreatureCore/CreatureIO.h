#pragma once

#include <filesystem>

namespace Creature
{
    class CCreature;
    namespace IO
    {
        class CCreatureIO
        {
        public:
            static bool SaveAsFile(
                const CCreature& creature, const std::filesystem::path& path);
            static bool LoadFromFile(const std::filesystem::path& path, CCreature& creature);
        };
    } // namespace IO
} // namespace Creature
