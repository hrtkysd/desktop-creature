#include "pch.h"
#include "Appearance.h"
#include "Application.h"
#include "CreatureIO.h"
#include "FileOperation.h"
#include "Motion.h"
#include "PartTransformBuilder.h"
#include "Skeleton.h"
#include "SpriteRenderDescription.h"
#include "Texture.h"

using namespace Creature;

namespace
{
    constexpr int CREATURE_WIDTH = 1280;
    constexpr int CREATURE_HEIGHT = 760;
    constexpr UINT_PTR RENDER_TIMER_ID = 1;
}

CAppRuntime::CAppRuntime() = default;

CAppRuntime::~CAppRuntime()
{
    if (m_hWorkspace)
    {
        DestroyWindow(m_hWorkspace);
        m_hWorkspace = nullptr;
    }
}

bool CAppRuntime::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    m_hWorkspace = CreateWorkspace(
        hInstance,
        CREATURE_WIDTH,
        CREATURE_HEIGHT,
        nCmdShow);
    if (!m_hWorkspace) return false;
    if (!InitializeGraphics(CREATURE_WIDTH, CREATURE_HEIGHT))
    {
        return false;
    }
    if (!LoadCreature()) return false;

    m_animationPlayer.Play();

    if (SetTimer(m_hWorkspace, RENDER_TIMER_ID, 16, nullptr) == 0)
    {
        m_animationPlayer.Stop();
        return false;
    }
    return true;
}

LRESULT CALLBACK CAppRuntime::WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    CAppRuntime* pApp = nullptr;

    if (message == WM_NCCREATE)
    {
        const auto createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pApp = reinterpret_cast<CAppRuntime*>(createStruct->lpCreateParams);

        SetWindowLongPtrW(
            hWnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pApp));
    }
    else
    {
        pApp = reinterpret_cast<CAppRuntime*>(
            GetWindowLongPtrW(
                hWnd,
                GWLP_USERDATA));
    }
    switch (message)
    {
    case WM_TIMER:
    {
        if (wParam != RENDER_TIMER_ID) break;
        constexpr float fDeltaTime = 1.0f / 60.0f;

        if (!pApp) return 0;
        if (pApp->Update(fDeltaTime))
        {
            pApp->Render();
        }
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps{};
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT client{};
        GetClientRect(hWnd, &client);

        HBRUSH background =
            CreateSolidBrush(RGB(0, 0, 0));

        FillRect(
            hdc,
            &client,
            background);

        DeleteObject(background);
        EndPaint(hWnd, &ps);

        return 0;
    }

    case WM_NCHITTEST:
        return HTTRANSPARENT;

    case WM_DESTROY:
        KillTimer(hWnd, RENDER_TIMER_ID);
        PostQuitMessage(0);
        return 0;
    case WM_NCDESTROY:
    {
        if (pApp && pApp->m_hWorkspace == hWnd)
        {
            pApp->m_hWorkspace = nullptr;
        }

        SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
        break;
    }
    default:
        break;
    }

    return DefWindowProc(
        hWnd,
        message,
        wParam,
        lParam
    );
}

int CAppRuntime::Run()
{
    MSG msg{};

    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

HWND CAppRuntime::CreateWorkspace(
    HINSTANCE hInstance,
    std::uint32_t width,
    std::uint32_t height,
    int nCmdShow)
{
    if (width == 0 || height == 0) return nullptr;

    constexpr wchar_t CLASS_NAME[] = L"DesktopCreature";

    WNDCLASSEX wc{};

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CAppRuntime::WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassEx(&wc)) return nullptr;

    auto hWnd = CreateWindowEx(
        WS_EX_LAYERED |
        WS_EX_TOPMOST |
        WS_EX_TOOLWINDOW |
        WS_EX_NOACTIVATE,
        CLASS_NAME,
        L"Creature",
        WS_POPUP,
        100,
        500,
        static_cast<int>(width),
        static_cast<int>(height),
        nullptr,
        nullptr,
        hInstance,
        this
    );
    if (hWnd == nullptr) return nullptr;

    SetLayeredWindowAttributes(
        hWnd,
        RGB(0, 0, 0),
        0,
        LWA_COLORKEY);

    ShowWindow(hWnd, nCmdShow);

    return hWnd;
}

bool CAppRuntime::InitializeGraphics(
    std::uint32_t width,
    std::uint32_t height)
{
    if (!m_renderer.Initialize(m_hWorkspace, width, height)) return false;
    m_textureCache.emplace(m_renderer.GetDevice());
    return true;
}

bool CAppRuntime::LoadCreature()
{
    const auto path = CFileOperation::ShowOpenCreatureDialog(m_hWorkspace);
    if (path.empty()) return false;
    auto loadCreature = m_creature.Clone();
    if (!IO::CCreatureIO::LoadFromFile(path, loadCreature)) return false;
    m_creature = std::move(loadCreature);
    return true;
}

bool CAppRuntime::Update(float fDeltaTime)
{
    const auto& motions = m_creature.GetMotions();
    if (motions.empty()) return false;

    const auto& motion = motions.front();
    const auto& skeleton = motion.GetSkeleton();

    m_animationPlayer.Update(fDeltaTime);
    m_animationPlayer.SamplePose(motion.GetAnimation(), skeleton);
    return true;
}

bool CAppRuntime::Render()
{
    if (!m_textureCache) return false;
    const auto& motions = m_creature.GetMotions();
    if (motions.empty()) return false;

    const auto& motion = motions.front();
    const auto& pose = m_animationPlayer.GetPose();

    if (!m_renderer.BeginFrame()) return false;
    m_renderer.BeginSprite();

    const auto& skeleton = motion.GetSkeleton();
    const auto& creatureAppearance = motion.GetAppearance();
    constexpr float creatureScale = 0.1f;

    const auto creatureToScreen =
        Math::CMatrix3x2::CreateScale(
            { creatureScale, creatureScale }) *
        Math::CMatrix3x2::CreateTranslation(
            { 640.0f, 380.0f });

    for (const auto& part : skeleton.Parts())
    {
        const auto appearance = creatureAppearance.FindByPartId(part.id);
        if (!appearance) continue;

        auto texture = m_textureCache->Load(appearance->texturePath);
        if (!texture) continue;

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

        m_renderer.DrawSprite(desc);
    }

    m_renderer.Present();

    return true;
}
