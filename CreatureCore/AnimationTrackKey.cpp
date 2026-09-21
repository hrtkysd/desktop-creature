#include "pch.h"
#include "Animation.h"
#include "AnimationTrackKey.h"

using namespace Creature;
using namespace Creature::Animation;

CAnimationTrackKey::CAnimationTrackKey(PartId partId, AnimationProperty eProperty)
    : m_partId(partId)
    , m_eProperty(eProperty)
{
}

Creature::PartId CAnimationTrackKey::GetPartId() const noexcept
{
    return m_partId;
}

AnimationProperty CAnimationTrackKey::GetProperty() const noexcept
{
    return m_eProperty;
}

bool CAnimationTrackKey::IsValid() const noexcept
{
    return
        m_partId != INVALID_PART_ID &&
        m_eProperty != AnimationProperty::None;
}

bool CAnimationTrackKey::operator==(const CAnimationTrackKey& other) const noexcept
{
    return
        m_partId == other.GetPartId() &&
        m_eProperty == other.GetProperty();
}
