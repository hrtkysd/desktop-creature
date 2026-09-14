#pragma once

#include <Windows.h>

namespace Creature
{
    struct Genome;
    struct CreaturePose;
}

class CCreatureRenderer
{
private:
    CCreatureRenderer() = delete;
    ~CCreatureRenderer() = delete;
public:
    static void Draw(
        HDC hdc,
        const RECT& client,
        const Creature::Genome& genome,
        const Creature::CreaturePose& pose);
};
