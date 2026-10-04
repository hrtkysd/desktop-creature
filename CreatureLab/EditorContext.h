#pragma once

#include "MotionId.h"
#include "PartId.h"
#include "Revision.h"
#include "UndoBuffer.h"

#include <cstdint>

enum class EditMode : std::uint8_t
{
    Select,
    Move,
    Rotate,
    Scale,
    Pivot
};

enum class SelectionType : std::uint8_t
{
    None,
    Creature,
    Motion,
    Part,
};

class CUndoScope;

class CEditorContext
{
    friend class CEditorController;
public:
    explicit CEditorContext(CUndoBuffer& undoBuffer);
public:
    void SelectCreature();
    void SelectPart(Creature::PartId partId);
    void SelectMotion(
        Creature::MotionId motionId);
    SelectionType GetSelectionType() const noexcept;

    CUndoScope CreateUndoScope() noexcept;
    void ResetHistory();
    bool CanUndo() const noexcept;
    bool CanRedo() const noexcept;
    Revision GetRevision() const noexcept;

    EditMode GetEditMode() const noexcept;
    void SetEditMode(EditMode mode);

    Creature::PartId GetPartId() const noexcept;
    Creature::MotionId GetMotionId() const noexcept;
private:
    void Undo();
    void Redo();
private:
    Creature::MotionId m_motionId
        = Creature::INVALID_MOTION_ID;
    Creature::PartId m_partId = Creature::INVALID_PART_ID;
    EditMode m_eEditMode = EditMode::Select;
    SelectionType m_eSelectionType = SelectionType::None;

    CUndoBuffer& m_undoBuffer;
};
