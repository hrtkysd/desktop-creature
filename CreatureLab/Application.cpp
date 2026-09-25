#include "pch.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Application.h"
#include "CreaturePose.h"
#include "TextureCache.h"

// third party
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

using namespace Creature;
using namespace Creature::Animation;

namespace
{
    constexpr wchar_t kWindowClassName[] = L"CreatureLabWindow";
    constexpr wchar_t kWindowTitle[] = L"Creature Lab";
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

CApp::CApp()
    : m_creatureEditor(m_creature)
    , m_undoBuffer(m_creature, 100)
    , m_editorContext(m_undoBuffer)
    , m_documentController(m_window, m_creature, m_documentContext)
    , m_editorController(m_creatureEditor, m_animationPlayer, m_editorContext)
    , m_labController(m_editorContext, m_documentController, m_editorController, m_creatureEditor)
    , m_menuBar(m_labController)
    , m_previewPanel(m_creatureEditor, m_editorContext)
    , m_creatureTreePanel(m_creatureEditor, m_editorContext)
    , m_animationPanel(m_animationPlayer, m_creatureEditor.GetAnimationEditor(), m_editorContext)
{
}

CApp::~CApp() { Shutdown(); }

bool CApp::Initialize(
    HINSTANCE hInstance,
    int nCmdShow)
{
    m_hInstance = hInstance;

    if (!CreateMainWindow(hInstance, nCmdShow)) return false;
    if (!CreateGraphics()) return false;

    InitializeCreature();

    if (!InitializeImGui()) return false;

    return true;
}

bool CApp::CreateMainWindow(
    HINSTANCE hInstance,
    int nCmdShow)
{
    WNDCLASSEXW wc{};

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CApp::WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = kWindowClassName;

    if (!RegisterClassExW(&wc)) return false;

    m_window.Create(hInstance, kWindowClassName, kWindowTitle, 1280, 800, this);

    ShowWindow(m_window.Handle(), nCmdShow);
    UpdateWindow(m_window.Handle());

    return true;
}

bool CApp::CreateGraphics()
{
    RECT rect{};
    GetClientRect(m_window.Handle(), &rect);

    const auto width = static_cast<std::uint32_t>(rect.right - rect.left);
    const auto height = static_cast<std::uint32_t>(rect.bottom - rect.top);
    if (!m_graphics.Initialize(m_window.Handle(), width, height)) return false;

    m_textureCache = std::make_shared<CTextureCache>(m_graphics.GetDevice());

    return true;
}

bool CApp::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplWin32_Init(m_window.Handle()))
    {
        return false;
    }

    if (!ImGui_ImplDX11_Init(
        m_graphics.GetDevice(),
        m_graphics.GetContext()))
    {
        ImGui_ImplWin32_Shutdown();
        return false;
    }

    m_bImGuiInitialized = true;

    return true;
}

void CApp::InitializeCreature()
{
    m_creatureEditor.SetName("Hamster");

    auto& skeletonEditor = m_creatureEditor.GetSkeletonEditor();
    const auto bodyId = skeletonEditor.AddPart("Body");
    const auto headId = skeletonEditor.AddPart("Head", bodyId);
    const auto eyesId = skeletonEditor.AddPart("Eyes", headId);
    const auto leftEarId = skeletonEditor.AddPart("LeftEar", headId);
    const auto rightEarId = skeletonEditor.AddPart("RightEar", headId);

    auto& appearanceEditor = m_creatureEditor.GetAppearanceEditor();
    appearanceEditor.SetTexture(bodyId, L"assets/body.png");
    appearanceEditor.SetTexture(headId, L"assets/head.png");
    appearanceEditor.SetTexture(eyesId, L"assets/eyes.png");
    appearanceEditor.SetTexture(leftEarId, L"assets/left_ear.png");
    appearanceEditor.SetTexture(rightEarId, L"assets/right_ear.png");

    CAnimationTrack headRotation(CAnimationTrackKey{ headId, AnimationProperty::Rotation });
    headRotation.AddOrUpdateKeyFrame({ 0.0f,  0.0f });
    headRotation.AddOrUpdateKeyFrame({ 0.5f,  0.1f });
    headRotation.AddOrUpdateKeyFrame({ 1.0f,  0.0f });
    const auto newId = m_creatureEditor.AddNewAnimation("idle");

    auto& animationsEditor = m_creatureEditor.GetAnimationEditor();
    animationsEditor.AddTrack(newId, std::move(headRotation));
}

int CApp::Run()
{
    MSG msg{};

    while (msg.message != WM_QUIT)
    {
        while (PeekMessageW(
            &msg,
            nullptr,
            0,
            0,
            PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            break;
        }

        Render();
    }

    return static_cast<int>(msg.wParam);
}

void CApp::Render()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    const auto viewport = ImGui::GetMainViewport();

    const ImGuiID dockspaceId = ImGui::GetID("CreatureLabDockSpace");

    if (ImGui::DockBuilderGetNode(dockspaceId) == nullptr)
    {
        ImGui::DockBuilderRemoveNode(dockspaceId);

        ImGui::DockBuilderAddNode(
            dockspaceId,
            ImGuiDockNodeFlags_DockSpace);

        ImGui::DockBuilderSetNodeSize(
            dockspaceId,
            viewport->WorkSize);

        ImGuiID mainDockId = dockspaceId;
        ImGuiID leftDockId = 0;
        ImGuiID downDockId = 0;

        ImGui::DockBuilderSplitNode(
            mainDockId,
            ImGuiDir_Left,
            0.25f,
            &leftDockId,
            &mainDockId);

        ImGui::DockBuilderSplitNode(
            mainDockId,
            ImGuiDir_Down,
            0.25f,
            &downDockId,
            &mainDockId);

        ImGui::DockBuilderDockWindow(
            "Creature",
            leftDockId);

        ImGui::DockBuilderDockWindow(
            "Animation",
            downDockId);

        ImGui::DockBuilderDockWindow(
            "Preview",
            mainDockId);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    ImGui::DockSpaceOverViewport(
        dockspaceId,
        viewport,
        ImGuiDockNodeFlags_None);

    m_menuBar.Draw(m_editorContext);
    m_creatureTreePanel.Draw();

    const auto animation = m_creature.FindReadonlyAnimationById(m_editorContext.GetAnimationId());
    const auto& skeleton = m_creature.GetReadonlySkeleton();
    const auto defaultPose = CCreaturePose::Default(skeleton);
    const auto* pose = &defaultPose;
    if (animation)
    {
        m_animationPlayer.Update(ImGui::GetIO().DeltaTime);
        m_animationPlayer.SamplePose(*animation, skeleton);

        m_animationPanel.Draw(*animation, skeleton);

        pose = &m_animationPlayer.GetPose();
    }


    m_previewPanel.Draw(*pose, *m_textureCache);
    ImGui::Render();

    if (!m_graphics.BeginFrame()) return;

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_graphics.Present();
}

void CApp::Shutdown()
{
    if (m_bImGuiInitialized)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        m_bImGuiInitialized = false;
    }

    m_graphics.Shutdown();

    if (m_hInstance)
    {
        UnregisterClassW(
            kWindowClassName,
            m_hInstance);

        m_hInstance = nullptr;
    }
}

LRESULT CALLBACK CApp::WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    CApp* pApp = nullptr;

    if (message == WM_NCCREATE)
    {
        const auto createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pApp = static_cast<CApp*>(createStruct->lpCreateParams);

        SetWindowLongPtrW(
            hWnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pApp));

        pApp->m_window.Attach(hWnd);
    }
    else
    {
        pApp = reinterpret_cast<CApp*>(
            GetWindowLongPtrW(
                hWnd,
                GWLP_USERDATA));
    }

    if (pApp)
    {
        return pApp->HandleMessage(
            hWnd,
            message,
            wParam,
            lParam);
    }

    return DefWindowProcW(
        hWnd,
        message,
        wParam,
        lParam);
}

LRESULT CApp::HandleMessage(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    if (m_bImGuiInitialized)
    {
        if (ImGui_ImplWin32_WndProcHandler(
            hWnd,
            message,
            wParam,
            lParam))
        {
            return TRUE;
        }
    }

    switch (message)
    {
    case WM_SIZE:
    {
        if (wParam != SIZE_MINIMIZED)
        {
            const auto width = static_cast<UINT>(LOWORD(lParam));
            const auto height = static_cast<UINT>(HIWORD(lParam));
            m_graphics.Resize(width, height);
        }
    }
    return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xFFF0) == SC_KEYMENU) return 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(
        hWnd,
        message,
        wParam,
        lParam);
}
