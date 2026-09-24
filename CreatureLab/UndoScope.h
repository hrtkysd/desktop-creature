#pragma once

#include "Creature.h"

class CUndoBuffer;

class CUndoScope
{
    friend class CUndoBuffer;
public:
    CUndoScope() = delete;
    CUndoScope(CUndoBuffer& buffer, Creature::CCreature&& before);
    ~CUndoScope();

    CUndoScope(const CUndoScope&) = delete;
    CUndoScope& operator=(const CUndoScope&) = delete;

    CUndoScope(CUndoScope&&);
    CUndoScope& operator=(CUndoScope&&) = delete;
public:
    void Cancel();
private:
    CUndoBuffer& m_buffer;
    Creature::CCreature m_before;
    bool m_isActive = true;
};
