#pragma once

#include <windef.h>

class CWindow
{
public:
    CWindow();
    ~CWindow();

    CWindow(const CWindow&) = delete;
    CWindow& operator=(const CWindow&) = delete;
public:
    HWND Handle() const noexcept;
    void Attach(HWND hWnd) noexcept;
    bool Create(
        HINSTANCE hInstance,
        const wchar_t* pszClassName,
        const wchar_t* pszTitle,
        int nWidth,
        int nHeight);

private:
    HWND m_hWnd = nullptr;
};
