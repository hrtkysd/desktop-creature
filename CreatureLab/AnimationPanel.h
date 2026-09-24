#pragma once

#include "AnimationId.h"
#include "AnimationProperty.h"
#include "AnimationTrackKey.h"

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
    }
}
class CEditorContext;

class CAnimationPanel
{
public:
    explicit CAnimationPanel(
        Creature::Animation::CAnimationPlayer& animationPlayer,
        Creature::Editor::CAnimationEditor& editor,
        CEditorContext& editorContext);
public:
    bool Draw(
        const Creature::Animation::CAnimation& animation,
        const Creature::CSkeleton& skeleton);
private:
    bool DrawAnimationProperties(
        const Creature::Animation::CAnimation& animation);
    void DrawCurrentTime(
        const Creature::Animation::CAnimation& animation);
    bool DrawTracks(
        const Creature::Animation::CAnimation& animation,
        const Creature::CSkeleton& skeleton);
    bool DrawAddTrack(
        const Creature::Animation::CAnimation& animation);
    bool DrawTrack(
        const Creature::Animation::CAnimation& animation,
        const Creature::Animation::CAnimationTrack& track);

private:
    Creature::Animation::CAnimationPlayer& m_animationPlayer;
    Creature::Editor::CAnimationEditor& m_editor;
    CEditorContext& m_editorContext;

    Creature::Animation::AnimationId m_animationId =
        Creature::Animation::INVALID_ANIMATION_ID;

    std::optional<Creature::Animation::CAnimationTrackKey> m_selectedTrackKey;

    Creature::Animation::AnimationProperty m_eNewTrackProperty =
        Creature::Animation::AnimationProperty::Rotation;
};
