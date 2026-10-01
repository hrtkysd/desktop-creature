#include "pch.h"
#include "StringConverter.h"
#include "Window.h"

#include <string>

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

bool CWindow::Create(
    HINSTANCE hInstance,
    const wchar_t* pszClassName,
    const std::string_view title,
    int nWidth,
    int nHeight,
    void* userData)
{
    m_hWnd = CreateWindowExW(
        0,
        pszClassName,
        StringConverter::Utf8ToWide(title).c_str(),
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

void CWindow::SetTitle(const std::string_view title)
{
    if (!m_hWnd) return;
    ::SetWindowText(m_hWnd, StringConverter::Utf8ToWide(title).c_str());
}
