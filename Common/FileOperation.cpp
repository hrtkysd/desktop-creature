#include "FileDialogFilter.h"
#include "FileOperation.h"
#include "StringConverter.h"

#include <ShObjIdl.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace
{
    class CComDialogFilters
    {
    public:
        explicit CComDialogFilters(const std::vector<CFileDialogFilter>& vecFilters)
        {
            m_vecName.reserve(vecFilters.size());
            m_vecPattern.reserve(vecFilters.size());

            for (const auto& filter : vecFilters)
            {
                m_vecName.push_back(StringConverter::Utf8ToWide(filter.Name()));
                m_vecPattern.push_back(StringConverter::Utf8ToWide(filter.Pattern()));
            }

            m_vecFilter.reserve(vecFilters.size());

            for (auto i = 0ULL; i < vecFilters.size(); ++i)
            {
                m_vecFilter.push_back({ m_vecName.at(i).c_str(), m_vecPattern.at(i).c_str() });
            }
        }

        bool Empty() const { return m_vecFilter.empty(); }

        UINT Size() const { return static_cast<UINT>(m_vecFilter.size()); }

        const COMDLG_FILTERSPEC* Data() const { return m_vecFilter.data(); }

    private:
        std::vector<std::wstring> m_vecName;
        std::vector<std::wstring> m_vecPattern;
        std::vector<COMDLG_FILTERSPEC> m_vecFilter;
    };
}

std::filesystem::path CFileOperation::ShowOpenDialog(
    HWND hWnd,
    const std::vector<CFileDialogFilter>& vecFilter)
{
    Microsoft::WRL::ComPtr<IFileOpenDialog> dialog;

    HRESULT hr = CoCreateInstance(
        CLSID_FileOpenDialog,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&dialog));

    if (FAILED(hr)) return {};

    const CComDialogFilters vecComFilter(vecFilter);
    if (!vecComFilter.Empty())
    {
        dialog->SetFileTypes(vecComFilter.Size(), vecComFilter.Data());
        dialog->SetFileTypeIndex(1);
    }

    hr = dialog->Show(hWnd);

    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) return {};

    if (FAILED(hr)) return {};

    ComPtr<IShellItem> item;

    if (FAILED(dialog->GetResult(&item))) return {};

    PWSTR pszPath = nullptr;

    if (FAILED(item->GetDisplayName(
        SIGDN_FILESYSPATH,
        &pszPath)))
    {
        return {};
    }

    std::filesystem::path path{ pszPath };

    CoTaskMemFree(pszPath);

    return path;
}

std::filesystem::path CFileOperation::ShowSaveDialog(
    HWND hWnd,
    std::string_view strDefaultExtension,
    std::string_view strDefaultFileName,
    const std::vector<CFileDialogFilter>& vecFilter)
{
    ComPtr<IFileSaveDialog> dialog;

    HRESULT hr = CoCreateInstance(
        CLSID_FileSaveDialog,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&dialog));

    if (FAILED(hr)) return {};

    const CComDialogFilters vecComFilter(vecFilter);
    if (!vecComFilter.Empty())
    {
        dialog->SetFileTypes(vecComFilter.Size(), vecComFilter.Data());
        dialog->SetFileTypeIndex(1);
    }
    const auto strDefaultExtensionW = StringConverter::Utf8ToWide(strDefaultExtension);
    dialog->SetDefaultExtension(strDefaultExtensionW.c_str());

    const auto strDefaultFileNameW = StringConverter::Utf8ToWide(strDefaultFileName);
    dialog->SetFileName(strDefaultFileNameW.c_str());

    hr = dialog->Show(hWnd);

    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) return {};
    if (FAILED(hr)) return {};

    ComPtr<IShellItem> item;

    hr = dialog->GetResult(&item);

    if (FAILED(hr)) return {};

    PWSTR pszPath = nullptr;

    hr = item->GetDisplayName(
        SIGDN_FILESYSPATH,
        &pszPath);

    if (FAILED(hr)) return {};

    std::filesystem::path path{ pszPath };

    CoTaskMemFree(pszPath);

    return path;
}
