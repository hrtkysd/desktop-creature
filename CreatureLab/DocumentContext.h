#pragma once

#include "DocumentStatus.h"

#include <filesystem>

class CDocumentContext
{
public:
    const std::filesystem::path& Path() const noexcept;
    void SetPath(std::filesystem::path&& path);

    void SetDocumentStatus(DocumentStatus status);
    DocumentStatus GetDocumentStatus() const;
    bool HasDocument() const noexcept;
private:
    DocumentStatus m_documentStatus = DocumentStatus::Empty;
    std::filesystem::path m_path;
};
