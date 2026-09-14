#include "pch.h"

#include "Animation.h"
#include "Creature.h"
#include "CreaturePose.h"
#include "Genome.h"
#include "GenomeGenerator.h"

using namespace Creature;
using namespace Creature::Animation;

CCreature::CCreature(uint32_t seed)
	: m_genome(std::make_unique<Genome>(CGenomeGenerator::Generate(seed)))
	, m_animation(std::make_unique<CAnimation>())
{
}

CCreature::~CCreature() = default;

const Genome& CCreature::GetGenome() const
{
	return *m_genome;
}

void CCreature::SetVelocity(Direction dir, float velocity)
{
	switch (dir)
	{
	case Direction::Horizontal:
		m_velocityX = velocity;
		break;
	case Direction::Vertical:
		m_velocityY = velocity;
		break;
	case Direction::Both:
		m_velocityX = velocity;
		m_velocityY = velocity;
		break;
	}
}

float CCreature::GetVelocityX(Direction dir)
{
	return m_velocityX;
}

float CCreature::GetPositionX()
{
	return m_posX;
}

float CCreature::GetPositionY()
{
	return m_posY;
}

const CAnimation& CCreature::GetAnimation()
{
	return *m_animation;
}

void CCreature::SetAnimationState(AnimationState state)
{
	m_animationState = state;
}

void CCreature::Update(float deltaTime)
{
	m_posX += m_velocityX * deltaTime;
	m_posY += m_velocityY * deltaTime;
	m_animation->Update(
		deltaTime,
		m_animationState,
		m_velocityX
	);
}
