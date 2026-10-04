#include "pch.h"

#include "Creature.h"
#include "CreatureEditor.h"
#include "MotionEditor.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"
#include "Transform2D.h"

using namespace Creature;
using namespace Creature::Editor;
using namespace Creature::Math;

namespace Creature
{
    // NOLINTBEGIN(bugprone-unchecked-optional-access)

    TEST(SkeletonEditorTest, SetPartTransformUpdatesTransform)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto id = skeletonEditor.AddPart("Body");
        ASSERT_NE(id, INVALID_PART_ID);

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(skeletonEditor.SetPartTransform(id, transform));

        const auto part = skeletonEditor.GetSkeleton().FindPartById(id);
        ASSERT_NE(part, nullptr);

        EXPECT_EQ(part->bindTransform.GetPosition(), Vec2({ 10.0f, 20.0f }));
        EXPECT_EQ(part->bindTransform.GetScale(), Vec2({ 2.0f, 3.0f }));
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), 0.5f);
    }

    TEST(SkeletonEditorTest, SetPartPositionUpdatesOnlyPosition)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto id = skeletonEditor.AddPart("Body");
        ASSERT_NE(id, INVALID_PART_ID);

        ASSERT_TRUE(skeletonEditor.SetPartRotation(id, 0.5f));
        ASSERT_TRUE(skeletonEditor.SetPartScale(id, { 2.0f, 3.0f }));
        ASSERT_TRUE(skeletonEditor.SetPartPivot(id, { 0.25f, 0.75f }));

        ASSERT_TRUE(skeletonEditor.SetPartPosition(id, { 10.0f, 20.0f }));

        const auto part = skeletonEditor.GetSkeleton().FindPartById(id);
        ASSERT_NE(part, nullptr);

        EXPECT_EQ(part->bindTransform.GetPosition(), Vec2({ 10.0f, 20.0f }));
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), 0.5f);
        EXPECT_EQ(part->bindTransform.GetScale(), Vec2({ 2.0f, 3.0f }));
        EXPECT_EQ(part->pivot, Vec2({ 0.25f, 0.75f }));
    }

    TEST(SkeletonEditorTest, SetOperationsRejectUnknownPart)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();

        constexpr PartId unknownId = 12345;

        EXPECT_FALSE(skeletonEditor.SetPartPosition(unknownId, { 1.0f, 2.0f }));
        EXPECT_FALSE(skeletonEditor.SetPartRotation(unknownId, 0.5f));
        EXPECT_FALSE(skeletonEditor.SetPartScale(unknownId, { 2.0f, 2.0f }));
        EXPECT_FALSE(skeletonEditor.SetPartPivot(unknownId, { 0.5f, 0.5f }));

        CTransform2D transform;
        EXPECT_FALSE(skeletonEditor.SetPartTransform(unknownId, transform));
        EXPECT_FALSE(skeletonEditor.SetPartPivotAndTransform(
            unknownId,
            { 0.5f, 0.5f },
            transform));
    }

    TEST(SkeletonEditorTest, SetPartPivotAndTransformUpdatesBoth)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto id = skeletonEditor.AddPart("Body");
        ASSERT_NE(id, INVALID_PART_ID);

        const Vec2 pivot{ 0.25f, 0.75f };

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(skeletonEditor.SetPartPivotAndTransform(id, pivot, transform));

        const auto part = skeletonEditor.GetSkeleton().FindPartById(id);
        ASSERT_NE(part, nullptr);

        EXPECT_EQ(part->pivot, pivot);
        EXPECT_EQ(part->bindTransform.GetPosition(), transform.GetPosition());
        EXPECT_EQ(part->bindTransform.GetScale(), transform.GetScale());
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), transform.GetRotation());
    }

    // NOLINTEND(bugprone-unchecked-optional-access)
}
