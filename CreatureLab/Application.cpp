#include "pch.h"

// third party
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "Application.h"
#include "GenomePanel.h"
#include "ImGuiWindowScope.h"
#include "PreviewPanel.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

namespace
{
    constexpr wchar_t kWindowClassName[] = L"CreatureLabWindow";
    constexpr wchar_t kWindowTitle[] = L"Creature Lab";
}

CApp::CApp()
    : m_hWnd(nullptr)
    , m_hInstance(nullptr)
    , m_bImGuiInitialized(false)
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

    m_hWnd = CreateWindowExW(
        0,
        kWindowClassName,
        kWindowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        800,
        nullptr,
        nullptr,
        hInstance,
        this);

    if (!m_hWnd) return false;

    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);

    return true;
}

bool CApp::CreateDeviceD3D()
{
    DXGI_SWAP_CHAIN_DESC desc{};

    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = m_hWnd;
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

    return SUCCEEDED(hr);
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

    if (!ImGui_ImplWin32_Init(m_hWnd))
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

        ImGui::DockBuilderSplitNode(
            mainDockId,
            ImGuiDir_Left,
            0.25f,
            &leftDockId,
            &mainDockId);

        ImGui::DockBuilderDockWindow(
            "Genome",
            leftDockId);

        ImGui::DockBuilderDockWindow(
            "Preview",
            mainDockId);

        ImGui::DockBuilderFinish(dockspaceId);
    }
    ImGui::DockSpaceOverViewport(
        dockspaceId,
        viewport,
        ImGuiDockNodeFlags_None);

    CGenomePanel::Draw(m_genome);
    CPreviewPanel::Draw(m_genome);

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

    ImGui_ImplDX11_RenderDrawData(
        ImGui::GetDrawData());

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

    if (m_hWnd)
    {
        DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }

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

        pApp->m_hWnd = hWnd;
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
