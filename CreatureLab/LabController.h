#pragma once

#include "Revision.h"
#include "UndoBuffer.h"

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
class CWindow;

enum class EditMode : std::uint8_t;

class CLabController
    : public IUndoBufferListener
{
public:
    explicit CLabController(
        CWindow& appWindow,
        CEditorContext& editorContext,
        CDocumentController& documentController,
        CEditorController& editorController,
        Creature::Editor::CCreatureEditor& creatureEditor);
protected:
    virtual void OnRevisionChanged(Revision revision) override;
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

    bool IsDirty() const noexcept;
private:
    void UpdateWindowTitle(Revision currentRevision);
private:
    Revision m_revision = 0;
    CEditorContext& m_editorContext;

    CWindow& m_appWindow;

    CDocumentController& m_documentController;
    CEditorController& m_editorController;

    Creature::Editor::CCreatureEditor& m_creatureEditor;
};
