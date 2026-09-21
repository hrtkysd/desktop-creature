#pragma once

namespace Creature
{
    class CSkeleton;
    namespace Animation
    {
        class CAnimation;
        class CAnimationPlayer;
    }
}
class CAnimationEditor;
class CEditorContext;

class CAnimationPanel
{
public:
    explicit CAnimationPanel(
        Creature::Animation::CAnimationPlayer& animationPlayer,
        CAnimationEditor& animationEditor,
        CEditorContext& editorContext);
public:
    bool Draw(
        const Creature::Animation::CAnimation& animation,
        const Creature::CSkeleton& skeleton);
private:
    Creature::Animation::CAnimationPlayer& m_animationPlayer;
    CAnimationEditor& m_editor;
    CEditorContext& m_editorContext;
};
