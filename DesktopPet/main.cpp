#include "pch.h"
#include "Application.h"
#include "ComInitializer.h"

using namespace Creature;

Animation::CAnimationPlayer g_player;
CCreature g_creature;
CGraphicsRenderer g_renderer;
std::optional<CTextureCache> g_cache;

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
