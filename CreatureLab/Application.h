#pragma once

#include "AnimationId.h"
#include "AnimationPanel.h"
#include "AnimationPlayer.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreatureTreePanel.h"
#include "DocumentContext.h"
#include "DocumentController.h"
#include "EditorContext.h"
#include "EditorController.h"
#include "Genome.h"
#include "LabController.h"
#include "MenuBar.h"
#include "PreviewPanel.h"
#include "Window.h"

#include <windef.h>

class CTextureCache;

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
    HINSTANCE m_hInstance = nullptr;
    CWindow m_window;

    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    Creature::Genome m_genome;
    Creature::CCreature m_creature;

    CCreatureTreePanel m_creatureTreePanel;

    std::shared_ptr<CTextureCache> m_textureCache;

    Creature::Animation::CAnimationPlayer m_animationPlayer;
    Creature::Animation::AnimationId m_idleAnimationId = Creature::Animation::INVALID_ANIMATION_ID;
    CPreviewPanel m_previewPanel;
    CAnimationPanel m_animationPanel;

    CEditorContext      m_editorContext;
    CDocumentContext    m_documentContext;

    Creature::Editor::CCreatureEditor     m_creatureEditor;

    CDocumentController m_documentController;
    CEditorController   m_editorController;
    CLabController      m_labController;

    CMenuBar m_menuBar;

    bool m_bImGuiInitialized = false;
};
