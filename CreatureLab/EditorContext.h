#pragma once

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

class CEditorContext
{
public:
    void SelectPart(Creature::PartId partId);
    Creature::PartId GetPartId() const noexcept;

    void SetEditMode(EditMode mode);
    EditMode GetEditMode() const noexcept;

private:
    Creature::PartId m_partId = Creature::INVALID_PART_ID;
    EditMode m_eEditMode = EditMode::Select;
};
