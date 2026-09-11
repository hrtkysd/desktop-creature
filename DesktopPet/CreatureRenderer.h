#pragma once

#include <Windows.h>

struct Genome;
struct CreaturePose;

class CCreatureRenderer
{
private:
    CCreatureRenderer() = delete;
    ~CCreatureRenderer() = delete;
public:
    static void Draw(
        HDC hdc,
        const RECT& client,
        const Genome& genome,
        const CreaturePose& pose);
};