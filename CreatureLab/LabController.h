#pragma once

#include <cstdint>

namespace Creature
{
    namespace Editor
    {
        class CCreatureEditor;
    }
}
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
        Creature::Editor::CCreatureEditor& creatureEditor);
public:
    bool SaveAs();
    bool LoadFrom();

    void PlayAnimation();
    void PauseAnimation();
    void StopAnimation();

    void SetEditMode(EditMode mode);
    EditMode GetEditMode() const noexcept;

    bool DeletePart();

    void Undo();
    void Redo();

private:
    CEditorContext& m_editorContext;

    CDocumentController& m_documentController;
    CEditorController& m_editorController;

    Creature::Editor::CCreatureEditor& m_creatureEditor;
};
