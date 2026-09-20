#pragma once

namespace Creature
{
    namespace Animation
    {
        class CAnimation;
        class CAnimationPlayer;
    }
}
class CAnimationEditor;

class CAnimationPanel
{
public:
    static bool Draw(
        const Creature::Animation::CAnimation& animation,
        Creature::Animation::CAnimationPlayer& animationPlayer,
        CAnimationEditor& animationEditor);
};
