#pragma once

#include <cstdint>

enum class Body : std::uint8_t
{
	Circle,
	Oval,
	Blob
};

enum class Eyes : std::uint8_t
{
	Mono,
	Dual,
	Tri,
};

enum class Legs : std::uint8_t
{
	None,
	Two,
	Four
};

enum class MouseReaction : std::uint8_t
{
	Ignore,
	Watch,
	Chase,
	Flee
};