#pragma once

#include "AnimationId.h"
#include "PartId.h"
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
    Part,
    Animation
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
    void SelectAnimation(
        Creature::Animation::AnimationId animationId);
    SelectionType GetSelectionType() const noexcept;

    CUndoScope CreateUndoScope() noexcept;
    void ClearHistory();
    bool CanUndo() const noexcept;
    bool CanRedo() const noexcept;

    EditMode GetEditMode() const noexcept;
    void SetEditMode(EditMode mode);

    Creature::PartId GetPartId() const noexcept;
    Creature::Animation::AnimationId GetAnimationId() const noexcept;
private:
    void Undo();
    void Redo();
private:
    Creature::Animation::AnimationId m_animationId;
    Creature::PartId m_partId = Creature::INVALID_PART_ID;
    EditMode m_eEditMode = EditMode::Select;
    SelectionType m_eSelectionType = SelectionType::None;

    CUndoBuffer& m_undoBuffer;
};
