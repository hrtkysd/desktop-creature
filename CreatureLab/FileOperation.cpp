#include "pch.h"
#include "FileOperation.h"

#include <Windows.h>
#include <shobjidl.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace
{
    const COMDLG_FILTERSPEC kFilters[] =
    {
        { L"Creature File", L"*.creature" },
        { L"All Files",     L"*.*" }
    };
}

std::filesystem::path CFileOperation::ShowOpenCreatureDialog(HWND hWnd)
{
    Microsoft::WRL::ComPtr<IFileOpenDialog> dialog;

    HRESULT hr = CoCreateInstance(
        CLSID_FileOpenDialog,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&dialog));

    if (FAILED(hr)) return {};

    dialog->SetFileTypes(
        static_cast<UINT>(std::size(kFilters)),
        kFilters);

    dialog->SetFileTypeIndex(1);

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

std::filesystem::path CFileOperation::ShowSaveCreatureDialog(HWND hWnd)
{
    ComPtr<IFileSaveDialog> dialog;

    HRESULT hr = CoCreateInstance(
        CLSID_FileSaveDialog,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&dialog));

    if (FAILED(hr)) return {};

    dialog->SetFileTypes(
        static_cast<UINT>(std::size(kFilters)),
        kFilters);

    dialog->SetFileTypeIndex(1);
    dialog->SetDefaultExtension(L"creature");
    dialog->SetFileName(L"creature.creature");

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
