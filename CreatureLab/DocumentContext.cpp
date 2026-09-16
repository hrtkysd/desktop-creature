#include "pch.h"
#include "DocumentContext.h"

const std::filesystem::path& CDocumentContext::Path() const noexcept
{
    return m_path;
}

void CDocumentContext::SetPath(std::filesystem::path&& path)
{
    m_path = std::move(path);
}
