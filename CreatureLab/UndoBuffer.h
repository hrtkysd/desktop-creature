#pragma once

#include <vector>

namespace Creature
{
    class CCreature;
}

class CUndoScope;

class CUndoBuffer
{
    friend class CUndoScope;
public:
    explicit CUndoBuffer(
        Creature::CCreature& creature,
        std::size_t maxHistory);

    CUndoScope CreateScope();

    bool CanUndo() const noexcept;
    bool CanRedo() const noexcept;

    void Undo();
    void Redo();

    void Clear();
private:
    void Push(Creature::CCreature&& state);

private:
    Creature::CCreature& m_creature;

    std::vector<Creature::CCreature> m_vecUndo;
    std::vector<Creature::CCreature> m_vecRedo;

    size_t m_maxHistoryCount;
};
