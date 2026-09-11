#include "Animation.h"
#include "Creature.h"
#include "CreaturePose.h"
#include "CreatureRenderer.h"
#include "BehaviorController.h"
#include "Features.h"
#include "Genome.h"

#include <cmath>
#include <Windows.h>
#include <chrono>

constexpr int CREATURE_WIDTH = 64;
constexpr int CREATURE_HEIGHT = 64;

CCreature g_creature(95647);
CBehaviorController g_controller;


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

        POINT mousePos{};
        GetCursorPos(&mousePos);

        InputState input{};

        input.mouseX =
            static_cast<float>(mousePos.x);

        input.mouseY =
            static_cast<float>(mousePos.y);

        input.leftButtonDown =
            (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

        //
        // 意思決定
        //
        g_controller.Update(
            g_creature,
            input,
            deltaTime
        );

        //
        // 実際の位置更新
        //
        g_creature.Update(deltaTime);

        //
        // Win32 Windowへ反映
        //
        SetWindowPos(
            hwnd,
            HWND_TOPMOST,
            static_cast<int>(g_creature.GetPositionX()),
            static_cast<int>(g_creature.GetPositionY()),
            CREATURE_WIDTH,
            CREATURE_HEIGHT,
            SWP_NOACTIVATE
        );

        InvalidateRect(
            hwnd,
            nullptr,
            FALSE
        );
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps{};
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT client{};
        GetClientRect(hwnd, &client);

        //
        // ColorKeyで透明になる背景
        //
        HBRUSH background =
            CreateSolidBrush(RGB(0, 0, 0));

        FillRect(
            hdc,
            &client,
            background);

        DeleteObject(background);

        const auto& pose =
            g_creature
            .GetAnimation()
            .GetPose();

        CCreatureRenderer::Draw(hdc, client, g_creature.GetGenome(), pose);

        EndPaint(hwnd, &ps);

        return 0;
    }

    case WM_NCHITTEST:
        // マウス操作を背後のアプリに通す
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
    constexpr wchar_t CLASS_NAME[] =
        L"DesktopCreature";

    WNDCLASS wc{};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
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

    if (!hwnd)
        return 0;

    SetLayeredWindowAttributes(
        hwnd,
        RGB(0, 0, 0),
        0,
        LWA_COLORKEY
    );

    ShowWindow(hwnd, SW_SHOW);

    SetTimer(
        hwnd,
        1,
        16, // 約60fps
        nullptr
    );

    MSG msg{};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}