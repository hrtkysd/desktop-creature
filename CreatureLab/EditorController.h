#pragma once

#include <cstdint>

namespace Creature
{
    class CCreature;

    namespace Animation
    {
        class CAnimationPlayer;
    }
    namespace Editor
    {
        class CCreatureEditor;
    }
}

class CEditorContext;
enum class EditorCommand : std::uint8_t;

class CEditorController
{
public:
    explicit CEditorController(
        Creature::Editor::CCreatureEditor& editor,
        Creature::Animation::CAnimationPlayer& animationPlayer,
        CEditorContext& context);
public:
    void Execute(EditorCommand command);

private:
    Creature::Editor::CCreatureEditor& m_editor;
    Creature::Animation::CAnimationPlayer& m_animationPlayer;
    CEditorContext& m_context;
};
