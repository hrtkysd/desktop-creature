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
    CApp app;

    if (!app.Initialize(hInstance, nCmdShow)) return -1;

    return app.Run();
}
