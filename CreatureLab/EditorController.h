#pragma once

#include <cstdint>

namespace Creature
{
    class CCreature;

    namespace Animation
    {
        class CAnimationPlayer;
    }
}

class CEditorContext;
enum class EditorCommand : std::uint8_t;

class CEditorController
{
public:
    explicit CEditorController(
        Creature::CCreature& creature,
        Creature::Animation::CAnimationPlayer& animationPlayer,
        CEditorContext& context);
public:
    void Execute(EditorCommand command);

private:
    Creature::CCreature& m_creature;
    Creature::Animation::CAnimationPlayer& m_animationPlayer;
    CEditorContext& m_context;
};
