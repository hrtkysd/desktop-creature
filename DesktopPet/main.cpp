#include "pch.h"
#include "Application.h"
#include "ComInitializer.h"

using namespace Creature;

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    PWSTR,
    int nShowCmd)
{
    CComInitializer comInit;
    if (!comInit.Succeeded()) return false;
    CAppRuntime app;
    if (!app.Initialize(hInstance, nShowCmd)) return false;
    return app.Run();
}
