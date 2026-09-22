#include "pch.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Appearance.h"
#include "Application.h"
#include "AnimationEditor.h"
#include "AnimationPanel.h"
#include "CreaturePose.h"
#include "Skeleton.h"
#include "Part.h"
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
    , m_animationEditor(m_creature)
    , m_documentController(m_window, m_creature, m_documentContext)
    , m_editorController(m_creature, m_animationPlayer, m_editorContext)
    , m_labController(m_editorContext, m_documentController, m_editorController, m_creatureEditor)
    , m_menuBar(m_labController)
    , m_previewPanel(m_creatureEditor, m_editorContext)
    , m_creatureTreePanel(m_creatureEditor, m_editorContext)
    , m_animationPanel(m_animationPlayer, m_animationEditor, m_editorContext)
{
}

CApp::~CApp() { Shutdown(); }

bool CApp::Initialize(
    HINSTANCE hInstance,
    int nCmdShow)
{
    m_hInstance = hInstance;

    if (!CreateMainWindow(hInstance, nCmdShow)) return false;
    if (!CreateDeviceD3D()) return false;
    if (!CreateRenderTarget()) return false;

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

bool CApp::CreateDeviceD3D()
{
    DXGI_SWAP_CHAIN_DESC desc{};

    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = m_window.Handle();
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    constexpr D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL featureLevel{};

    const auto hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels,
        static_cast<UINT>(std::size(featureLevels)),
        D3D11_SDK_VERSION,
        &desc,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &featureLevel,
        m_deviceContext.GetAddressOf());

    if (FAILED(hr) || !m_device) return false;

    m_textureCache = std::make_shared<CTextureCache>(m_device.Get());
    return true;
}

bool CApp::CreateRenderTarget()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    auto hr = m_swapChain->GetBuffer(
        0,
        IID_PPV_ARGS(backBuffer.GetAddressOf()));

    if (FAILED(hr)) return false;

    hr = m_device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        m_renderTargetView.GetAddressOf());

    return SUCCEEDED(hr);
}

void CApp::CleanupRenderTarget()
{
    m_renderTargetView.Reset();
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
        m_device.Get(),
        m_deviceContext.Get()))
    {
        ImGui_ImplWin32_Shutdown();
        return false;
    }

    m_bImGuiInitialized = true;

    return true;
}

void CApp::InitializeCreature()
{
    m_creature.SetName("Hamster");

    auto& skeleton = m_creature.GetSkeleton();
    const auto bodyId = skeleton.AddPart("Body");
    const auto headId = skeleton.AddPart("Head", bodyId);
    const auto eyesId = skeleton.AddPart("Eyes", headId);
    const auto leftEarId = skeleton.AddPart("LeftEar", headId);
    const auto rightEarId = skeleton.AddPart("RightEar", headId);

    auto& appearance = m_creature.GetAppearance();
    appearance.SetTexture(bodyId, L"assets/body.png");
    appearance.SetTexture(headId, L"assets/head.png");
    appearance.SetTexture(eyesId, L"assets/eyes.png");
    appearance.SetTexture(leftEarId, L"assets/left_ear.png");
    appearance.SetTexture(rightEarId, L"assets/right_ear.png");

    CAnimationTrack headRotation(CAnimationTrackKey{ headId, AnimationProperty::Rotation });
    headRotation.AddOrUpdateKeyFrame({ 0.0f,  0.0f });
    headRotation.AddOrUpdateKeyFrame({ 0.5f,  0.1f });
    headRotation.AddOrUpdateKeyFrame({ 1.0f,  0.0f });
    const auto newId = m_creatureEditor.AddNewAnimation("idle");
    m_animationEditor.AddAnimationTrack(newId, std::move(headRotation));
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

    auto animation = m_creature.FindAnimationById(m_editorContext.GetAnimationId());
    const auto& skeleton = m_creature.GetSkeleton();
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

    constexpr float clearColor[] =
    {
        0.1f,
        0.1f,
        0.1f,
        1.0f
    };

    const auto pRenderTarget = m_renderTargetView.Get();

    m_deviceContext->OMSetRenderTargets(
        1,
        &pRenderTarget,
        nullptr);

    m_deviceContext->ClearRenderTargetView(
        m_renderTargetView.Get(),
        clearColor);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_swapChain->Present(1, 0);
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

    CleanupRenderTarget();

    m_swapChain.Reset();
    m_deviceContext.Reset();
    m_device.Reset();

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
        if (m_device && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();

            const auto width = static_cast<UINT>(LOWORD(lParam));
            const auto height = static_cast<UINT>(HIWORD(lParam));

            const auto hr = m_swapChain->ResizeBuffers(
                0,
                width,
                height,
                DXGI_FORMAT_UNKNOWN,
                0);
            if (SUCCEEDED(hr)) CreateRenderTarget();
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
