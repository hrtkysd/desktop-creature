#pragma once

#include "PartId.h"

#include <cstdint>

namespace Creature
{
    namespace Animation
    {
        enum class AnimationProperty : std::uint8_t;

        class CAnimationTrackKey
        {
        public:
            CAnimationTrackKey() = delete;
            explicit CAnimationTrackKey(PartId partId, AnimationProperty eProperty);

            Creature::PartId GetPartId() const noexcept;
            AnimationProperty GetProperty() const noexcept;
            bool IsValid() const noexcept;
            bool operator==(const CAnimationTrackKey& other) const noexcept;

        private:
            PartId m_partId;
            AnimationProperty m_eProperty;
        };
    }
}
