#include "FileDialogFilter.h"

CFileDialogFilter::CFileDialogFilter(
    const std::string& strName,
    const std::string& strPattern)
    : m_strName(strName)
    , m_strPattern(strPattern)
{

}

const std::string& CFileDialogFilter::Name() const
{
    return m_strName;
}

const std::string& CFileDialogFilter::Pattern() const
{
    return m_strPattern;
}
