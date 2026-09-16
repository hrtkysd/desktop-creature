#include "pch.h"
#include "Creature.h"
#include "CreatureIO.h"
#include "FileOperation.h"
#include "DocumentCommand.h"
#include "DocumentContext.h"
#include "DocumentController.h"
#include "Window.h"

using namespace Creature;
using namespace Creature::IO;

CDocumentController::CDocumentController(CWindow& appWindow, CCreature& creature, CDocumentContext& context)
    : m_appWindow(appWindow)
    , m_creature(creature)
    , m_context(context)
{
}

const std::filesystem::path& CDocumentController::Path() const noexcept
{
    return m_context.Path();
}

void CDocumentController::Execute(DocumentCommand command)
{
    switch (command)
    {
    case DocumentCommand::SaveAs:
    {
        auto path = CFileOperation::ShowSaveCreatureDialog(m_appWindow.Handle());
        if (path.empty() || !CCreatureIO::SaveAsFile(m_creature, path)) return;
        m_context.SetPath(std::move(path));
    }
    break;
    case DocumentCommand::LoadFrom:
    {
        const auto path = CFileOperation::ShowOpenCreatureDialog(m_appWindow.Handle());
        if (path.empty()) return;
        CCreature creature;
        if (!CCreatureIO::LoadFromFile(path, creature)) return;
        m_creature = std::move(creature);
    }
    break;
    default:
        break;
    }
}
