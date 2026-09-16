#pragma once

#include <cstdint>

enum class EditorCommand : std::uint8_t
{
    PlayAnimation,
    PauseAnimation,
    StopAnimation,
    DeletePart,
    ResetTransform,
    ToForward,
    ToBackward,
    Undo,
    Redo
};
