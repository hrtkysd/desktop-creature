#pragma once

namespace Creature
{
    namespace Animation
    {
        class CAnimation;
    }
}

class CAnimationPanel
{
public:
    static bool Draw(Creature::Animation::CAnimation& animation);
};
