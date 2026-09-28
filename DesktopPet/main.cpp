#include "Animation.h"
#include "AnimationPlayer.h"
#include "Appearance.h"
#include "BehaviorController.h"
#include "ComInitializer.h"
#include "Creature.h"
#include "CreatureIO.h"
#include "CreaturePose.h"
#include "FileOperation.h"
#include "GraphicsRenderer.h"
#include "PartTransformBuilder.h"
#include "Skeleton.h"
#include "SpriteRenderDescription.h"
#include "Texture.h"
#include "TextureCache.h"

#include <optional>
#include <Windows.h>

constexpr int CREATURE_WIDTH = 1280;
constexpr int CREATURE_HEIGHT = 760;

CBehaviorController g_controller;

using namespace Creature;

Animation::CAnimationPlayer g_player;
CCreature g_creature;
CGraphicsRenderer g_renderer;
std::optional<CTextureCache> g_cache;

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        constexpr float deltaTime = 1.0f / 60.0f;

        const auto& animations = g_creature.GetReadonlyAnimations();
        if (animations.empty()) return 0;

        const auto& animation = animations.front();
        const auto& skeleton = g_creature.GetReadonlySkeleton();
        const auto& creatureAppearance = g_creature.GetReadonlyAppearance();

        g_player.Update(deltaTime);
        g_player.SamplePose(animation, skeleton);

        const auto& pose = g_player.GetPose();

        if (!g_renderer.BeginFrame()) return 0;
        g_renderer.BeginSprite();

        for (const auto& part : skeleton.Parts())
        {
            const auto appearance = creatureAppearance.FindByPartId(part.id);
            if (!appearance) continue;

            auto texture = g_cache->Load(appearance->texturePath);
            if (!texture) continue;

            constexpr float creatureScale = 0.1f;

            const auto creatureToScreen =
                Math::CMatrix3x2::CreateScale(
                    { creatureScale, creatureScale }) *
                Math::CMatrix3x2::CreateTranslation(
                    { 640.0f, 380.0f });

            const auto worldTransform =
                Math::CPartTransformBuilder::BuildWorld(
                    part,
                    skeleton,
                    pose);

            const auto screenTransform =
                worldTransform * creatureToScreen;
            CSpriteRenderDescription desc
            {
                texture,
                {
                    static_cast<float>(texture->GetWidth()),
                    static_cast<float>(texture->GetHeight())
                },
                screenTransform,
            };

            g_renderer.DrawSprite(desc);
        }

        g_renderer.Present();
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps{};
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT client{};
        GetClientRect(hwnd, &client);

        HBRUSH background =
            CreateSolidBrush(RGB(0, 0, 0));

        FillRect(
            hdc,
            &client,
            background);

        DeleteObject(background);
        EndPaint(hwnd, &ps);

        return 0;
    }

    case WM_NCHITTEST:
        return HTTRANSPARENT;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(
        hwnd,
        message,
        wParam,
        lParam
    );
}

int WINAPI wWinMain(
    HINSTANCE instance,
    HINSTANCE,
    PWSTR,
    int)
{
    CComInitializer comInit;
    if (!comInit.Succeeded()) return 0;

    constexpr wchar_t CLASS_NAME[] =
        L"DesktopCreature";

    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        WS_EX_LAYERED |
        WS_EX_TOPMOST |
        WS_EX_TOOLWINDOW |
        WS_EX_NOACTIVATE,
        CLASS_NAME,
        L"Creature",
        WS_POPUP,
        100,
        500,
        CREATURE_WIDTH,
        CREATURE_HEIGHT,
        nullptr,
        nullptr,
        instance,
        nullptr
    );
    if (!hWnd) return 0;
    const auto filepath = CFileOperation::ShowOpenCreatureDialog(hWnd);
    if (filepath.empty()) return 0;

    SetLayeredWindowAttributes(
        hWnd,
        RGB(0, 0, 0),
        0,
        LWA_COLORKEY);

    ShowWindow(hWnd, SW_SHOW);

    if (!IO::CCreatureIO::LoadFromFile(filepath, g_creature))
    {
        return 0;
    }

    if (!g_renderer.Initialize(hWnd, 1280, 760)) return 0;
    g_cache.emplace(g_renderer.GetDevice());
    g_player.Play();

    SetTimer(hWnd, 1, 16, nullptr); // about 60 fps.

    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
