#pragma once

namespace Creature
{
    struct Genome;
}

class CPreviewPanel
{
public:
    static void Draw(const Creature::Genome& genome);
};
