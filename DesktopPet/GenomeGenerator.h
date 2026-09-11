#pragma once

#include <cstdint>

struct Genome;

class CGenomeGenerator
{
private:
	CGenomeGenerator() = delete;
	~CGenomeGenerator() = delete;
public:
	static Genome Generate(uint32_t seed);
};
