#pragma once

#include "Animation.h"
#include "AnimationPlayer.h"

#include "Creature.h"
#include "Genome.h"

#include <filesystem>
#include <windef.h>

class CCreatureEditor;
class CDocumentContext;
class CDocumentController;
class CEditorContext;
class CEditorController;
class CLabController;
class CMenuBar;
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

    void SetupEditor();

    void CleanupRenderTarget();
    void Shutdown();

    void Render();

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

    Creature::Animation::CAnimationPlayer m_animationPlayer;
    Creature::Animation::CAnimation m_idleAnimation;
    std::unique_ptr<CPreviewPanel> m_previewPanel;

    std::unique_ptr<CEditorContext>      m_editorContext;
    std::unique_ptr<CDocumentContext>    m_documentContext;

    std::unique_ptr<CCreatureEditor>     m_creatureEditor;

    std::unique_ptr<CDocumentController> m_documentController;
    std::unique_ptr<CEditorController>   m_editorController;
    std::unique_ptr<CLabController>      m_labController;

    std::unique_ptr<CMenuBar>            m_menuBar;

    bool m_bImGuiInitialized;
};
