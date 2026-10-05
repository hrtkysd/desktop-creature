#pragma once

#include <string>

class CFileDialogFilter
{
public:
    CFileDialogFilter() = delete;
    explicit CFileDialogFilter(
        const std::string& strName,
        const std::string& strPattern);
public:
    const std::string& Name() const;
    const std::string& Pattern() const;

private:
    std::string m_strName;
    std::string m_strPattern;
};
