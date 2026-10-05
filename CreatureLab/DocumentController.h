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
    void CreateNewDocument();

    std::optional<Creature::CCreature> LoadCreature() const;
    bool SaveCreature(const Creature::CCreature& creature);

    std::optional<std::filesystem::path> LoadApperance() const;
private:
    CWindow& m_appWindow;
    CDocumentContext& m_context;
};
