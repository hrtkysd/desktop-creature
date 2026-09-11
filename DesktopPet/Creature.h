#pragma once

#include <memory>

enum class AnimationState;
class CAnimation;

enum class Direction : uint8_t
{
    Horizontal,
    Vertical,
    Both
};
struct Genome;

class CCreature
{
public:
	explicit CCreature(uint32_t seed);
    ~CCreature();
public:
    const Genome& GetGenome() const;
    void SetVelocity(Direction dir, float velocity);
    float GetVelocityX(Direction dir);

    float GetPositionX();
    float GetPositionY();
    const CAnimation& GetAnimation();
    void SetAnimationState(AnimationState state);
    void Update(float deltaTime);
private:
	std::unique_ptr<const Genome> m_genome;
    std::unique_ptr<CAnimation> m_animation;

    AnimationState m_animationState;

    float m_posX = 0.0f;
    float m_posY = 0.0f;

    float m_velocityX = 0.0f;
    float m_velocityY = 0.0f;
};