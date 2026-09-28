#include "pch.h"
#include "Application.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    const auto hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) return 0;

    CApp app;
    if (!app.Initialize(hInstance, nCmdShow)) return -1;

    const auto result = app.Run();

    CoUninitialize();

    return result;
}
