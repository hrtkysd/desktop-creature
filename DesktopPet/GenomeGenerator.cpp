#include "Genome.h"
#include "GenomeGenerator.h"

#include <random>

namespace
{
    float RandomFloat(
        std::mt19937& rng,
        float min,
        float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }
    
    int RandomInt(
        std::mt19937& rng,
        int min,
        int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }


    template<typename T>
    static T RandomEnum(
        std::mt19937& rng,
        T minValue,
        T maxValue)
    {
        const int min = static_cast<int>(minValue);
        const int max = static_cast<int>(maxValue);
        return static_cast<T>(RandomInt(rng, min, max));
    }

    MouseReaction GetMouseReaction(Genome& genome)
    {
        if (genome.fear > 0.75f) return MouseReaction::Flee;
        if (genome.curiosity > 0.75f) return MouseReaction::Chase;
        if (genome.curiosity > 0.4f) return MouseReaction::Watch;
        return MouseReaction::Ignore;
    }
}
Genome CGenomeGenerator::Generate(uint32_t seed)
{
    std::mt19937 rng(seed);

    Genome genome{};

    genome.seed = seed;

    genome.body = RandomEnum<Body>(rng, Body::Circle, Body::Blob);
    genome.eyes = RandomEnum<Eyes>(rng, Eyes::Mono, Eyes::Tri);
    genome.legs = RandomEnum<Legs>(rng, Legs::None, Legs::Four);

    genome.scale = RandomFloat(rng, 0.7f, 1.4f);

    genome.speed = RandomFloat(rng, 30.0f, 120.0f);
    genome.jump = RandomFloat(rng, 100.0f, 300.0f);

    genome.curiosity = RandomFloat(rng, 0.0f, 1.0f);

    genome.fear = RandomFloat(rng, 0.0f, 1.0f);

    genome.sleepiness = RandomFloat(rng, 0.0f, 1.0f);

    genome.mouseReaction = GetMouseReaction(genome);

    return genome;
}
