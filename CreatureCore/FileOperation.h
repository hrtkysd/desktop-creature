#pragma once

#include <filesystem>
#include <Windows.h>

namespace Creature
{
    namespace IO
    {
        class CFileOperation
        {
        public:
            static std::filesystem::path ShowOpenCreatureDialog(HWND hWnd);
            static std::filesystem::path ShowSaveCreatureDialog(HWND hWnd);
        };
    }
}
