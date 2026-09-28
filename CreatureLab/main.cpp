#include "pch.h"
#include "Application.h"
#include "ComInitializer.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    CComInitializer comInit;
    if (!comInit.Succeeded()) return 0;

    CApp app;
    if (!app.Initialize(hInstance, nCmdShow)) return -1;
    return app.Run();
}
