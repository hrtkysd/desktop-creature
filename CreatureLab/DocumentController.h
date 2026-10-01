#pragma once

#include "Creature.h"

#include <filesystem>
#include <optional>

class CWindow;
class CDocumentContext;

class CDocumentController
{
public:
    explicit CDocumentController(
        CWindow& appWindow,
        CDocumentContext& context);
public:
    const std::filesystem::path& Path() const noexcept;
    std::optional<Creature::CCreature> Load() const;
    bool Save(const Creature::CCreature& creature);
private:
    CWindow& m_appWindow;
    CDocumentContext& m_context;
};
