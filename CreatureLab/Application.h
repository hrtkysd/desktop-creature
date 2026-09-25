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
#include "GraphicsDevice.h"
#include "LabController.h"
#include "MenuBar.h"
#include "PreviewPanel.h"
#include "UndoBuffer.h"
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
    bool CreateGraphics();

    bool InitializeImGui();
    void InitializeCreature();

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

    CGraphicsDevice m_graphics;

    Creature::Genome m_genome;
    Creature::CCreature m_creature;
    Creature::Editor::CCreatureEditor m_creatureEditor;

    CUndoBuffer m_undoBuffer;

    CCreatureTreePanel m_creatureTreePanel;

    std::shared_ptr<CTextureCache> m_textureCache;

    Creature::Animation::CAnimationPlayer m_animationPlayer;
    Creature::Animation::AnimationId m_idleAnimationId = Creature::Animation::INVALID_ANIMATION_ID;
    CPreviewPanel m_previewPanel;
    CAnimationPanel m_animationPanel;

    CEditorContext      m_editorContext;
    CDocumentContext    m_documentContext;

    CDocumentController m_documentController;
    CEditorController   m_editorController;
    CLabController      m_labController;

    CMenuBar m_menuBar;

    bool m_bImGuiInitialized = false;
};
