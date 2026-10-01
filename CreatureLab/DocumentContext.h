#pragma once

#include <filesystem>

class CDocumentContext
{
public:
    const std::filesystem::path& Path() const noexcept;
    void SetPath(std::filesystem::path&& path);
private:
    std::filesystem::path m_path;
};
