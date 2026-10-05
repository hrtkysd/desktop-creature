#include "pch.h"
#include "ApplicationInformation.h"
#include "CreatureIO.h"
#include "DocumentContext.h"
#include "DocumentController.h"
#include "FileDialogFilter.h"
#include "FileOperation.h"
#include "Window.h"

using namespace Creature;
using namespace Creature::IO;

namespace
{
    std::vector<CFileDialogFilter> CreatureFilters()
    {
        return
        {
             CFileDialogFilter{ "Creature File", "*.creature" },
             CFileDialogFilter{ "All Files", "*.*" }
        };
    }

    std::vector<CFileDialogFilter> AppearanceFilters()
    {
        return
        {
            CFileDialogFilter{ "Image Files", "*.png;*.jpg;*.jpeg;*.bmp" },
            CFileDialogFilter{ "PNG Image", "*.png" },
            CFileDialogFilter{ "JPEG Image", "*.jpg;*.jpeg" },
            CFileDialogFilter{ "Bitmap Image", "*.bmp" }
        };
    }
}

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

std::optional<Creature::CCreature> CDocumentController::LoadCreature() const
{

    const auto path = CFileOperation::ShowOpenDialog(m_appWindow.Handle(), CreatureFilters());
    if (path.empty()) return {};

    CCreature creature;
    if (!CCreatureIO::LoadFromFile(path, creature)) return {};
    m_context.SetDocumentStatus(DocumentStatus::File);
    return creature;
}

bool CDocumentController::SaveCreature(const Creature::CCreature& creature)
{
    auto path = CFileOperation::ShowSaveDialog(
        m_appWindow.Handle(),
        Information::File::DefaultCreatureFileExtension,
        Information::File::DefaultCreatureFileName,
        CreatureFilters());
    if (path.empty() || !CCreatureIO::SaveAsFile(creature, path)) return false;
    m_context.SetPath(std::move(path));
    m_context.SetDocumentStatus(DocumentStatus::File);
    return true;
}

std::optional<std::filesystem::path> CDocumentController::LoadAppearance() const
{
    return CFileOperation::ShowOpenDialog(m_appWindow.Handle(), AppearanceFilters());
}
