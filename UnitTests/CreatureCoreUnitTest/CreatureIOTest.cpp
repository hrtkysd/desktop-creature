#include "pch.h"
#include "Appearance.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreatureIO.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "SkeletonEditor.h"

#include <filesystem>
#include <system_error>

using namespace Creature;
using namespace Creature::Editor;
using namespace Creature::IO;

namespace
{
    class CScopedTestFile
    {
    public:
        explicit CScopedTestFile(std::filesystem::path path)
            : m_path(std::move(path))
        {
            std::error_code ec;
            std::filesystem::remove(m_path, ec);
        }

        ~CScopedTestFile()
        {
            std::error_code ec;
            std::filesystem::remove(m_path, ec);
        }

        const std::filesystem::path& Path() const
        {
            return m_path;
        }

    private:
        std::filesystem::path m_path;
    };
}

// NOLINTBEGIN(bugprone-unchecked-optional-access)

TEST(CreatureIOTest, SaveAndLoadPreservesAppearanceZOrder)
{
    CCreature creature;
    CCreatureEditor editor(creature);

    editor.SetName("TestCreature");

    const auto motionId = editor.AddNewMotion("Idle");
    ASSERT_NE(motionId, INVALID_MOTION_ID);

    auto motionEditor = editor.MotionEditor(motionId);
    ASSERT_TRUE(motionEditor.has_value());

    auto skeletonEditor = motionEditor->SkeletonEditor();

    const auto bodyId = skeletonEditor.AddPart("Body");
    const auto headId = skeletonEditor.AddPart("Head", bodyId);
    const auto eyesId = skeletonEditor.AddPart("Eyes", headId);

    ASSERT_NE(bodyId, INVALID_PART_ID);
    ASSERT_NE(headId, INVALID_PART_ID);
    ASSERT_NE(eyesId, INVALID_PART_ID);

    auto appearanceEditor = motionEditor->AppearanceEditor();

    appearanceEditor.AddPart(bodyId, L"assets/body.png");
    appearanceEditor.AddPart(headId, L"assets/head.png");
    appearanceEditor.AddPart(eyesId, L"assets/eyes.png");

    // Change the order from:
    //
    // Body(0), Head(1), Eyes(2)
    //
    // to:
    //
    // Eyes(0), Body(1), Head(2)
    ASSERT_TRUE(appearanceEditor.MoveBackward(eyesId));
    ASSERT_TRUE(appearanceEditor.MoveBackward(eyesId));

    const auto& appearanceBeforeSave =
        motionEditor->GetMotion().GetAppearance();

    ASSERT_EQ(appearanceBeforeSave.Parts().size(), 3u);

    EXPECT_EQ(appearanceBeforeSave.Parts()[0].partId, eyesId);
    EXPECT_EQ(appearanceBeforeSave.Parts()[0].zOrder, 0u);

    EXPECT_EQ(appearanceBeforeSave.Parts()[1].partId, bodyId);
    EXPECT_EQ(appearanceBeforeSave.Parts()[1].zOrder, 1u);

    EXPECT_EQ(appearanceBeforeSave.Parts()[2].partId, headId);
    EXPECT_EQ(appearanceBeforeSave.Parts()[2].zOrder, 2u);

    const CScopedTestFile file
    {
        std::filesystem::temp_directory_path() /
        "CreatureIO_ZOrderRoundTrip.creature"
    };

    ASSERT_TRUE(CCreatureIO::SaveAsFile(creature, file.Path()));

    CCreature loadedCreature;

    ASSERT_TRUE(CCreatureIO::LoadFromFile(file.Path(), loadedCreature));

    const auto loadedMotion = loadedCreature.FindMotionById(motionId);
    ASSERT_NE(loadedMotion, nullptr);

    const auto& loadedAppearance = loadedMotion->GetAppearance();
    ASSERT_EQ(loadedAppearance.Parts().size(), 3u);

    EXPECT_EQ(loadedAppearance.Parts()[0].partId, eyesId);
    EXPECT_EQ(loadedAppearance.Parts()[0].zOrder, 0u);
    EXPECT_EQ(loadedAppearance.Parts()[0].texturePath, std::filesystem::path{ L"assets/eyes.png" });

    EXPECT_EQ(loadedAppearance.Parts()[1].partId, bodyId);
    EXPECT_EQ(loadedAppearance.Parts()[1].zOrder, 1u);
    EXPECT_EQ(loadedAppearance.Parts()[1].texturePath, std::filesystem::path{ L"assets/body.png" });

    EXPECT_EQ(loadedAppearance.Parts()[2].partId, headId);
    EXPECT_EQ(loadedAppearance.Parts()[2].zOrder, 2u);
    EXPECT_EQ(loadedAppearance.Parts()[2].texturePath, std::filesystem::path{ L"assets/head.png" });
}
// NOLINTEND(bugprone-unchecked-optional-access)
