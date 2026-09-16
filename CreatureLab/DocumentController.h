#pragma once

#include "DocumentCommand.h"

#include <cstdint>
#include <filesystem>

namespace Creature
{
    class CCreature;
}

class CWindow;
class CDocumentContext;
enum class DocumentCommad : std::uint8_t;

class CDocumentController
{
public:
    explicit CDocumentController(CWindow& appWindow, Creature::CCreature& creature, CDocumentContext& context);
public:
    const std::filesystem::path& Path() const noexcept;

    void Execute(DocumentCommand command);
private:
    CWindow& m_appWindow;
    Creature::CCreature& m_creature;
    CDocumentContext& m_context;
};
