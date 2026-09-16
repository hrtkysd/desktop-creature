#pragma once

#include <cstdint>

class CCreatureEditor;
class CDocumentController;
class CEditorController;
class CEditorContext;

enum class EditMode : std::uint8_t;

class CLabController
{
public:
    explicit CLabController(
        CEditorContext& editorContext,
        CDocumentController& documentController,
        CEditorController& editorController,
        CCreatureEditor& creatureEditor);
public:
    void SaveAs();
    void LoadFrom();

    void PlayAnimation();
    void PauseAnimation();
    void StopAnimation();

    void SetEditMode(EditMode mode);

    CCreatureEditor& CreatureEditor();

private:
    CEditorContext& m_editorContext;

    CDocumentController& m_documentController;
    CEditorController& m_editorController;

    CCreatureEditor& m_creatureEditor;
};
