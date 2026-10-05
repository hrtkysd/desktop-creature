#pragma once

#include <filesystem>
#include <vector>
#include <string_view>
#include <Windows.h>

class CFileDialogFilter;

class CFileOperation
{
public:
    static std::filesystem::path ShowOpenDialog(
        HWND hWnd,
        const std::vector<CFileDialogFilter>& vecFilter);
    static std::filesystem::path ShowSaveDialog(
        HWND hWnd,
        std::string_view strDefaultExtension,
        std::string_view strDefaultFileName,
        const std::vector<CFileDialogFilter>& vecFilter);
};
