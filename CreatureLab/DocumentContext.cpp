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

void CDocumentContext::SetDocumentStatus(DocumentStatus status)
{
    m_documentStatus = status;
}

DocumentStatus CDocumentContext::GetDocumentStatus() const
{
    return m_documentStatus;
}

bool CDocumentContext::HasDocument() const noexcept
{
    return m_documentStatus != DocumentStatus::Empty;
}
