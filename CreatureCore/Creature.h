#pragma once

#include <memory>

enum class Direction : uint8_t
{
    Horizontal,
    Vertical,
    Both
};

namespace Creature
{
    namespace Animation
    {
        class CAnimation;
        enum class AnimationState;
    }

    struct Genome;

    class CCreature
    {
    public:
        explicit CCreature(uint32_t seed);
        ~CCreature();
    public:
        const Creature::Genome& GetGenome() const;
        void SetVelocity(Direction dir, float velocity);
        float GetVelocityX(Direction dir);

        float GetPositionX();
        float GetPositionY();
        const Animation::CAnimation& GetAnimation();
        void SetAnimationState(Animation::AnimationState state);
        void Update(float deltaTime);
    private:
        std::unique_ptr<const Creature::Genome> m_genome;
        std::unique_ptr<Creature::Animation::CAnimation> m_animation;

        Animation::AnimationState m_animationState;

        float m_posX = 0.0f;
        float m_posY = 0.0f;

        float m_velocityX = 0.0f;
        float m_velocityY = 0.0f;
    };
}
