#pragma once

#include "Creature.h"
#include "Genome.h"

#include <filesystem>
#include <windef.h>

class CTextureCache;
class CPreviewPanel;

class CApp final
{
public:
    CApp();
    ~CApp();

    CApp(const CApp&) = delete;
    CApp& operator=(const CApp&) = delete;

public:
    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    int Run();

private:
    bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
    bool CreateDeviceD3D();
    bool CreateRenderTarget();
    bool InitializeImGui();
    void InitializeCreature();

    void CleanupRenderTarget();
    void Shutdown();

    void Render();
    void DrawMenuBar();

    LRESULT HandleMessage(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam);

    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    Creature::Genome m_genome;
    Creature::CCreature m_creature;

    std::shared_ptr<CTextureCache> m_textureCache;

    std::unique_ptr<CPreviewPanel> m_previewPanel;
    std::filesystem::path m_creatureFilePath;
    bool m_bImGuiInitialized;
};
