#pragma once

#include <filesystem>
#include <windef.h>

namespace Creature
{
    class CCreature;
}

class CDocumentContext
{
public:
    const std::filesystem::path& Path() const noexcept;
    void SetPath(std::filesystem::path&& path);
private:
    std::filesystem::path m_path;
};
