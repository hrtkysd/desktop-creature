#pragma once

#include "AnimationProperty.h"
#include "AnimationTrackKey.h"
#include "UndoScope.h"

#include <optional>

namespace Creature
{
    class CSkeleton;
    namespace Animation
    {
        class CAnimation;
        class CAnimationPlayer;
        class CAnimationTrack;
    }
    namespace Editor
    {
        class CAnimationEditor;
        class CCreatureEditor;
    }
}
class CEditorContext;

class CAnimationPanel
{
public:
    explicit CAnimationPanel(
        Creature::Editor::CCreatureEditor& editor,
        Creature::Animation::CAnimationPlayer& animationPlayer,
        CEditorContext& editorContext);
public:
    bool Draw(
        const Creature::Animation::CAnimation& animation,
        const Creature::CSkeleton& skeleton);
private:
    bool DrawAnimationProperties(
        Creature::Editor::CAnimationEditor& editor,
        const Creature::Animation::CAnimation& animation);
    void DrawCurrentTime(
        const Creature::Animation::CAnimation& animation);
    bool DrawTracks(
        Creature::Editor::CAnimationEditor& editor,
        const Creature::Animation::CAnimation& animation,
        const Creature::CSkeleton& skeleton);
    bool DrawAddTrack(
        Creature::Editor::CAnimationEditor& editor,
        const Creature::Animation::CAnimation& animation);
    bool DrawTrack(
        Creature::Editor::CAnimationEditor& editor,
        const Creature::Animation::CAnimation& animation,
        const Creature::Animation::CAnimationTrack& track);

private:
    Creature::Editor::CCreatureEditor& m_editor;
    Creature::Animation::CAnimationPlayer& m_animationPlayer;
    CEditorContext& m_editorContext;

    std::optional<CUndoScope> m_undoScope;

    std::optional<Creature::Animation::CAnimationTrackKey> m_selectedTrackKey;

    Creature::Animation::AnimationProperty m_eNewTrackProperty =
        Creature::Animation::AnimationProperty::Rotation;
};
