#pragma once

#include "Features.h"

namespace Creature
{
    struct Genome
    {
        uint32_t seed = 0;

        Body body = Body::Circle;
        Eyes eyes = Eyes::Mono;
        Legs legs = Legs::None;

        MouseReaction mouseReaction = MouseReaction::Ignore;

        float scale = 1.0f;

        float bodyLength = 1.0f;
        float headScale = 1.0f;
        float legLength = 1.0f;

        float speed = 60.0f;
        float jump = 100.0f;
        float curiosity = 0.5f;
        float fear = 0.5f;
        float sleepiness = 0.5f;
    };
} // namespace Creature
