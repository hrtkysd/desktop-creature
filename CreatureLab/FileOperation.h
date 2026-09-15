#pragma once

#include <filesystem>
#include <windef.h>

class CFileOperation
{
public:
    static std::filesystem::path ShowOpenCreatureDialog(HWND hWnd);
    static std::filesystem::path ShowSaveCreatureDialog(HWND hWnd);
};
