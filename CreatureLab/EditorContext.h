#pragma once

#include "Animation.h"
#include "Part.h"

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

class CEditorContext
{
public:
    void SelectCreature();
    void SelectPart(Creature::PartId partId);
    void SelectAnimation(
        Creature::Animation::AnimationId animationId);
    SelectionType GetSelectionType() const noexcept;


    EditMode GetEditMode() const noexcept;
    void SetEditMode(EditMode mode);

    Creature::PartId GetPartId() const noexcept;
    Creature::Animation::AnimationId GetAnimationId() const noexcept;

private:
    Creature::Animation::AnimationId m_animationId;
    Creature::PartId m_partId = Creature::INVALID_PART_ID;
    EditMode m_eEditMode = EditMode::Select;
    SelectionType m_eSelectonType = SelectionType::None;
};
