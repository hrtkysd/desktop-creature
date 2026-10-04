#include "pch.h"

#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "MotionId.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"

#include <limits>

using namespace Creature;
using namespace Creature::Editor;

namespace Creature
{
    // NOLINTBEGIN(bugprone-unchecked-optional-access)

    TEST(CreatureEditorTest, SetNameUpdatesCreatureName)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        editor.SetName("TestCreature");

        EXPECT_EQ(creature.GetName(), "TestCreature");
    }

    TEST(CreatureEditorTest, AddNewMotionAddsMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddNewMotion("Idle");
        ASSERT_NE(id, INVALID_MOTION_ID);

        const auto motion = creature.FindMotionById(id);
        ASSERT_NE(motion, nullptr);

        EXPECT_EQ(motion->GetName(), "Idle");
    }

    TEST(CreatureEditorTest, AddNewMotionGeneratesUniqueIds)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto idleId = editor.AddNewMotion("Idle");
        const auto walkId = editor.AddNewMotion("Walk");
        const auto sleepId = editor.AddNewMotion("Sleep");

        EXPECT_NE(idleId, INVALID_MOTION_ID);
        EXPECT_NE(walkId, INVALID_MOTION_ID);
        EXPECT_NE(sleepId, INVALID_MOTION_ID);

        EXPECT_NE(idleId, walkId);
        EXPECT_NE(idleId, sleepId);
        EXPECT_NE(walkId, sleepId);
    }

    TEST(CreatureEditorTest, AddMotionWithIdUpdatesNextGeneratedId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr MotionId explicitId = 100;

        ASSERT_EQ(editor.AddMotionWithId(explicitId, "Imported"), explicitId);
        EXPECT_EQ(editor.AddNewMotion("Idle"), explicitId + 1);
    }

    TEST(CreatureEditorTest, AddMotionWithIdRejectsDuplicateId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr MotionId id = 10;

        ASSERT_EQ(editor.AddMotionWithId(id, "Idle"), id);

        EXPECT_EQ(editor.AddMotionWithId(id, "Walk"), INVALID_MOTION_ID);
        EXPECT_EQ(creature.GetMotions().size(), 1u);
    }

    TEST(CreatureEditorTest, AddMotionWithIdRejectsInvalidId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddMotionWithId(INVALID_MOTION_ID, "Invalid");

        EXPECT_EQ(id, INVALID_MOTION_ID);
        EXPECT_TRUE(creature.GetMotions().empty());
    }

    TEST(CreatureEditorTest, AddMotionWithIdRejectsMaxValue)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        EXPECT_EQ(
            editor.AddMotionWithId(std::numeric_limits<MotionId>::max(), "Invalid"),
            INVALID_MOTION_ID);
    }

    TEST(CreatureEditorTest, RemoveMotionRemovesSpecifiedMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto idleId = editor.AddNewMotion("Idle");
        const auto walkId = editor.AddNewMotion("Walk");

        ASSERT_NE(idleId, INVALID_MOTION_ID);
        ASSERT_NE(walkId, INVALID_MOTION_ID);

        ASSERT_TRUE(editor.RemoveMotion(idleId));

        EXPECT_EQ(creature.FindMotionById(idleId), nullptr);
        EXPECT_NE(creature.FindMotionById(walkId), nullptr);
        EXPECT_EQ(creature.GetMotions().size(), 1u);
    }

    TEST(CreatureEditorTest, RemoveMotionReturnsFalseForUnknownId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        EXPECT_FALSE(editor.RemoveMotion(12345));
    }

    TEST(CreatureTest, CloneCreatesIndependentCopy)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        editor.SetName("Original");

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto clone = creature.Clone();
        EXPECT_EQ(clone.GetName(), "Original");
        EXPECT_NE(clone.FindMotionById(motionId), nullptr);

        CCreatureEditor cloneEditor(clone);
        cloneEditor.SetName("Clone");

        EXPECT_EQ(creature.GetName(), "Original");
        EXPECT_EQ(clone.GetName(), "Clone");
    }

    TEST(CreatureTest, CloneOwnsIndependentMotionState)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        const auto bodyId = motionEditor->SkeletonEditor().AddPart("Body");
        ASSERT_NE(bodyId, INVALID_PART_ID);

        auto clone = creature.Clone();
        CCreatureEditor cloneEditor(clone);

        auto cloneMotionEditor = cloneEditor.MotionEditor(motionId);
        ASSERT_TRUE(cloneMotionEditor.has_value());

        const auto headId = cloneMotionEditor->SkeletonEditor().AddPart("Head");
        ASSERT_NE(headId, INVALID_PART_ID);

        const auto originalMotion = creature.FindMotionById(motionId);
        const auto clonedMotion = clone.FindMotionById(motionId);

        ASSERT_NE(originalMotion, nullptr);
        ASSERT_NE(clonedMotion, nullptr);

        EXPECT_EQ(originalMotion->GetSkeleton().Parts().size(), 1u);
        EXPECT_EQ(clonedMotion->GetSkeleton().Parts().size(), 2u);
    }

    TEST(CreatureTest, ClonePreservesNextMotionId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr MotionId explicitId = 100;
        ASSERT_EQ(editor.AddMotionWithId(explicitId, "Imported"), explicitId);

        auto clone = creature.Clone();
        CCreatureEditor cloneEditor(clone);

        EXPECT_EQ(cloneEditor.AddNewMotion("Walk"), explicitId + 1);
    }

    // NOLINTEND(bugprone-unchecked-optional-access)
}
