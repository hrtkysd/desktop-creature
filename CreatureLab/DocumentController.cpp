#include "pch.h"
#include "CreatureIO.h"
#include "DocumentContext.h"
#include "DocumentController.h"
#include "FileOperation.h"
#include "Window.h"

using namespace Creature;
using namespace Creature::IO;

CDocumentController::CDocumentController(
    CWindow& appWindow,
    CDocumentContext& context)
    : m_appWindow(appWindow)
    , m_context(context)
{
}

const std::filesystem::path& CDocumentController::Path() const noexcept
{
    return m_context.Path();
}

void CDocumentController::CreateNewDocument()
{
    m_context.SetPath({});
    m_context.SetDocumentStatus(DocumentStatus::New);
}

std::optional<Creature::CCreature> CDocumentController::Load() const
{
    const auto path = CFileOperation::ShowOpenCreatureDialog(m_appWindow.Handle());
    if (path.empty()) return {};

    CCreature creature;
    if (!CCreatureIO::LoadFromFile(path, creature)) return {};
    m_context.SetDocumentStatus(DocumentStatus::File);
    return creature;
}

bool CDocumentController::Save(const Creature::CCreature& creature)
{
    auto path = CFileOperation::ShowSaveCreatureDialog(m_appWindow.Handle());
    if (path.empty() || !CCreatureIO::SaveAsFile(creature, path)) return false;
    m_context.SetPath(std::move(path));
    m_context.SetDocumentStatus(DocumentStatus::File);
    return true;
}
