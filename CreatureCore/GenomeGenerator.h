#pragma once

#include <cstdint>

namespace Creature
{
    struct Genome;
}

class CGenomeGenerator
{
private:
	CGenomeGenerator() = delete;
	~CGenomeGenerator() = delete;
public:
	static Creature::Genome Generate(uint32_t seed);
};
