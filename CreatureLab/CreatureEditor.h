#pragma once

#include "Part.h"

class CCreatureDocument;

namespace Creature
{
    class CCreature;

    namespace Math
    {
        struct Vec2;
    }
}

class CCreatureEditor
{
public:
    explicit CCreatureEditor(Creature::CCreature& creature);

    void SetPartRotation(Creature::PartId id, float rotation);
    void SetPartScale(Creature::PartId id, const Creature::Math::Vec2& scale);
    void SetPartPivot(Creature::PartId id, const Creature::Math::Vec2& pivot);
    bool DeletePart(Creature::PartId id);

private:
    Creature::CCreature& m_creature;
};
