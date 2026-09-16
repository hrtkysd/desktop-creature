#pragma once

#include "DocumentCommand.h"

#include <cstdint>
#include <filesystem>
#include <windef.h>

namespace Creature
{
    class CCreature;
}

class CDocumentContext;
enum class DocumentCommad : std::uint8_t;

class CDocumentController
{
public:
    explicit CDocumentController(HWND hAppWnd, Creature::CCreature& creature, CDocumentContext& context);
public:
    const std::filesystem::path& Path() const noexcept;

    void Execute(DocumentCommand command);
private:
    HWND m_hAppWnd;
    Creature::CCreature& m_creature;
    CDocumentContext& m_context;
};
