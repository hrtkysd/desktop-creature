#include "pch.h"
#include "Window.h"

CWindow::CWindow() = default;

CWindow::~CWindow()
{
    if (m_hWnd == nullptr) return;
    ::DestroyWindow(m_hWnd);
    m_hWnd = nullptr;
}

HWND CWindow::Handle() const noexcept
{
    return m_hWnd;
}

bool CWindow::Create(HINSTANCE hInstance, const wchar_t* pszClassName, const wchar_t* pszTitle, int nWidth, int nHeight, void* userData)
{
    m_hWnd = CreateWindowExW(
        0,
        pszClassName,
        pszTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nWidth,
        nHeight,
        nullptr,
        nullptr,
        hInstance,
        userData);
    return m_hWnd != nullptr;
}
