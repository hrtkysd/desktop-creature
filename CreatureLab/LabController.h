#pragma once

#include "Revision.h"
#include "UndoBuffer.h"

#include <cstdint>
#include <filesystem>
#include <optional>

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
    void OnRevisionChanged(Revision revision) override;
public:
    void CreateNew();
    bool SaveCreatureAs();
    bool LoadCreatureFrom();

    std::optional<std::filesystem::path> LoadAppearance();

    void PlayAnimation();
    void PauseAnimation();
    void StopAnimation();

    void SetEditMode(EditMode mode);
    EditMode GetEditMode() const noexcept;

    bool DeletePart();

    bool CanMoveForward() const;
    bool CanMoveBackward() const;

    bool MoveForward();
    bool MoveBackward();

    void Undo();
    void Redo();

    bool IsDirty() const noexcept;
private:
    void UpdateWindowTitle();

private:
    Revision m_revision = 0;

    CWindow& m_appWindow;
    CEditorContext& m_editorContext;
    CDocumentController& m_documentController;
    CEditorController& m_editorController;

    Creature::Editor::CCreatureEditor& m_creatureEditor;
};
