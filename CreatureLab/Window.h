#pragma once

#include <string_view>
#include <Windows.h>

class CWindow
{
public:
    CWindow();
    ~CWindow();

    CWindow(const CWindow&) = delete;
    CWindow& operator=(const CWindow&) = delete;
public:
    HWND Handle() const noexcept;
    bool Create(
        HINSTANCE hInstance,
        const wchar_t* pszClassName,
        const std::string_view title,
        int nWidth,
        int nHeight,
        void* userData);
    void SetTitle(const std::string_view title);
private:
    HWND m_hWnd = nullptr;
};
