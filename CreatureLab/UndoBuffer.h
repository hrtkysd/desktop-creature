#pragma once

#include "Revision.h"

#include <vector>

namespace Creature
{
    class CCreature;
}

class CUndoScope;

class IUndoBufferListener
{
public:
    virtual ~IUndoBufferListener() = default;
    virtual void OnRevisionChanged(Revision revision) = 0;
};

class CUndoBuffer
{
    friend class CUndoScope;
public:
    explicit CUndoBuffer(
        Creature::CCreature& creature,
        std::size_t maxHistory);
    ~CUndoBuffer();
public:
    CUndoScope CreateScope();

    bool CanUndo() const noexcept;
    bool CanRedo() const noexcept;

    void Undo();
    void Redo();

    void ResetHistory();

    Revision GetCurrentRevision() const noexcept;
    void SetListener(IUndoBufferListener* listener) noexcept;
private:
    void Push(Creature::CCreature&& state);
    void SetCurrentRevision(
        Revision revision,
        bool isNotify);
private:
    Creature::CCreature& m_creature;

    struct SnapshotImpl;
    std::vector<SnapshotImpl> m_vecUndo;
    std::vector<SnapshotImpl> m_vecRedo;

    Revision m_currentRevision = 0;
    Revision m_nextRevision = 1;

    size_t m_maxHistoryCount;

    IUndoBufferListener* m_listener = nullptr;
};
