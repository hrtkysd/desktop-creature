#include "pch.h"

#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Appearance.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "MotionId.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"

#include <limits>

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

namespace Creature
{
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
    class MotionEditorTests : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            m_motionId = m_creatureEditor.AddNewMotion("Idle");
            ASSERT_NE(m_motionId, INVALID_MOTION_ID);

            auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
            ASSERT_TRUE(motionEditor.has_value());

            m_partId = motionEditor->SkeletonEditor().AddPart("Body");
            ASSERT_NE(m_partId, INVALID_PART_ID);
        }

        std::optional<CMotionEditor> GetMotionEditor()
        {
            return m_creatureEditor.MotionEditor(m_motionId);
        }

        const CMotion* GetMotion() const
        {
            return m_creature.FindMotionById(m_motionId);
        }

    protected:
        CCreature m_creature;
        CCreatureEditor m_creatureEditor{ m_creature };

        MotionId m_motionId = INVALID_MOTION_ID;
        PartId m_partId = INVALID_PART_ID;
    };

    TEST_F(MotionEditorTests, EmptyTrackCanBeAdded)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();

        const CAnimationTrackKey key{
            m_partId,
            AnimationProperty::PositionX
        };

        EXPECT_TRUE(editor.AddTrack(CAnimationTrack{ key }));
    }

    TEST_F(MotionEditorTests, SetDurationAcceptsPositiveDuration)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(1.0f));

        const auto* motion = GetMotion();
        ASSERT_NE(motion, nullptr);

        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 1.0f);
    }

    TEST_F(MotionEditorTests, SetDurationRejectsNegativeDuration)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();

        EXPECT_FALSE(editor.SetDuration(-1.0f));
    }

    TEST_F(MotionEditorTests, SetDurationRejectsDurationShorterThanExistingKeyFrame)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(2.0f));

        const CAnimationTrackKey trackKey{
            m_partId,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));
        ASSERT_TRUE(editor.AddOrUpdateKeyFrame(trackKey, { 1.5f, 10.0f }));

        EXPECT_FALSE(editor.SetDuration(1.0f));

        const auto* motion = GetMotion();
        ASSERT_NE(motion, nullptr);

        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 2.0f);
    }

    TEST_F(MotionEditorTests, DuplicateTrackIsRejected)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();

        const CAnimationTrackKey trackKey{
            m_partId,
            AnimationProperty::Rotation
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));
        EXPECT_FALSE(editor.AddTrack(CAnimationTrack{ trackKey }));

        const auto* motion = GetMotion();
        ASSERT_NE(motion, nullptr);

        EXPECT_EQ(motion->GetAnimation().GetAnimationTracks().size(), 1u);
    }

    TEST_F(MotionEditorTests, KeyFramePastDurationIsRejected)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(1.0f));

        const CAnimationTrackKey trackKey{
            m_partId,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        EXPECT_FALSE(editor.AddOrUpdateKeyFrame(trackKey, { 1.1f, 10.0f }));
    }

    TEST_F(MotionEditorTests, NegativeKeyFrameTimeIsRejected)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(1.0f));

        const CAnimationTrackKey trackKey{
            m_partId,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        EXPECT_FALSE(editor.AddOrUpdateKeyFrame(trackKey, { -0.1f, 10.0f }));
    }

    TEST(MotionEditorTest, MotionEditorRejectsInvalidAndUnknownIds)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        EXPECT_FALSE(editor.MotionEditor(INVALID_MOTION_ID).has_value());
        EXPECT_FALSE(editor.MotionEditor(12345).has_value());
    }

    TEST(MotionEditorTest, MotionEditorsMutateTheirMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto partId = skeletonEditor.AddPart("Body");
        ASSERT_NE(partId, INVALID_PART_ID);

        auto appearanceEditor = motionEditor->AppearanceEditor();
        appearanceEditor.SetTexture(partId, L"body.png");

        const CAnimationTrackKey trackKey{
            partId,
            AnimationProperty::PositionX
        };

        auto animationEditor = motionEditor->AnimationEditor();
        ASSERT_TRUE(animationEditor.SetDuration(2.0f));
        ASSERT_TRUE(animationEditor.AddTrack(CAnimationTrack{ trackKey }));

        const auto* motion = creature.FindMotionById(motionId);
        ASSERT_NE(motion, nullptr);

        EXPECT_NE(motion->GetSkeleton().FindPartById(partId), nullptr);
        EXPECT_NE(motion->GetAppearance().FindByPartId(partId), nullptr);
        EXPECT_EQ(motion->GetAnimation().GetAnimationTracks().size(), 1u);
        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 2.0f);
    }

    TEST(MotionTest, AddNewMotionSetsIdNameAndEmptyContents)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        const auto* motion = creature.FindMotionById(motionId);
        ASSERT_NE(motion, nullptr);

        EXPECT_EQ(motion->GetMotionId(), motionId);
        EXPECT_EQ(motion->GetName(), "Idle");
        EXPECT_TRUE(motion->GetSkeleton().Parts().empty());
        EXPECT_TRUE(motion->GetAnimation().GetAnimationTracks().empty());
        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 0.0f);
    }

    TEST(MotionTest, MotionsHaveIndependentSkeletonsAndAnimations)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto firstId = editor.AddNewMotion("First");
        const auto secondId = editor.AddNewMotion("Second");

        ASSERT_NE(firstId, INVALID_MOTION_ID);
        ASSERT_NE(secondId, INVALID_MOTION_ID);

        auto firstMotionEditor = editor.MotionEditor(firstId);
        auto secondMotionEditor = editor.MotionEditor(secondId);

        ASSERT_TRUE(firstMotionEditor.has_value());
        ASSERT_TRUE(secondMotionEditor.has_value());

        const auto firstPartId = firstMotionEditor->SkeletonEditor().AddPart("Body");
        ASSERT_NE(firstPartId, INVALID_PART_ID);

        const CAnimationTrackKey trackKey{
            firstPartId,
            AnimationProperty::Rotation
        };

        ASSERT_TRUE(firstMotionEditor->AnimationEditor().AddTrack(CAnimationTrack{ trackKey }));

        const auto* firstMotion = creature.FindMotionById(firstId);
        const auto* secondMotion = creature.FindMotionById(secondId);

        ASSERT_NE(firstMotion, nullptr);
        ASSERT_NE(secondMotion, nullptr);

        EXPECT_EQ(firstMotion->GetSkeleton().Parts().size(), 1u);
        EXPECT_TRUE(secondMotion->GetSkeleton().Parts().empty());
        EXPECT_EQ(firstMotion->GetAnimation().GetAnimationTracks().size(), 1u);
        EXPECT_TRUE(secondMotion->GetAnimation().GetAnimationTracks().empty());
    }

    TEST(MotionTest, CreatureCloneDeepCopiesMotionContents)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto partId = skeletonEditor.AddPart("Body");
        ASSERT_NE(partId, INVALID_PART_ID);

        motionEditor->AppearanceEditor().SetTexture(partId, L"body.png");

        const CAnimationTrackKey trackKey{
            partId,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(motionEditor->AnimationEditor().AddTrack(CAnimationTrack{ trackKey }));

        auto clone = creature.Clone();

        CCreatureEditor cloneEditor(clone);
        auto clonedMotionEditor = cloneEditor.MotionEditor(motionId);
        ASSERT_TRUE(clonedMotionEditor.has_value());

        ASSERT_TRUE(clonedMotionEditor->SkeletonEditor().RemovePart(partId));

        const auto* originalMotion = creature.FindMotionById(motionId);
        const auto* clonedMotion = clone.FindMotionById(motionId);

        ASSERT_NE(originalMotion, nullptr);
        ASSERT_NE(clonedMotion, nullptr);

        EXPECT_NE(originalMotion->GetSkeleton().FindPartById(partId), nullptr);
        EXPECT_NE(originalMotion->GetAppearance().FindByPartId(partId), nullptr);
        EXPECT_NE(originalMotion->GetAnimation().FindAnimationTrack(trackKey), nullptr);

        EXPECT_EQ(clonedMotion->GetSkeleton().FindPartById(partId), nullptr);
        EXPECT_EQ(clonedMotion->GetAppearance().FindByPartId(partId), nullptr);
        EXPECT_EQ(clonedMotion->GetAnimation().FindAnimationTrack(trackKey), nullptr);
    }

    TEST(MotionEditorTest, RemovingPartCascadesOnlyWithinItsMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto firstId = editor.AddNewMotion("First");
        const auto secondId = editor.AddNewMotion("Second");

        ASSERT_NE(firstId, INVALID_MOTION_ID);
        ASSERT_NE(secondId, INVALID_MOTION_ID);

        auto firstMotionEditor = editor.MotionEditor(firstId);
        auto secondMotionEditor = editor.MotionEditor(secondId);

        ASSERT_TRUE(firstMotionEditor.has_value());
        ASSERT_TRUE(secondMotionEditor.has_value());

        auto firstSkeletonEditor = firstMotionEditor->SkeletonEditor();
        const auto firstParentId = firstSkeletonEditor.AddPart("Body");
        const auto firstChildId = firstSkeletonEditor.AddPart("Head", firstParentId);

        auto secondSkeletonEditor = secondMotionEditor->SkeletonEditor();
        const auto secondParentId = secondSkeletonEditor.AddPart("Body");
        const auto secondChildId = secondSkeletonEditor.AddPart("Head", secondParentId);

        ASSERT_NE(firstParentId, INVALID_PART_ID);
        ASSERT_NE(firstChildId, INVALID_PART_ID);
        ASSERT_NE(secondParentId, INVALID_PART_ID);
        ASSERT_NE(secondChildId, INVALID_PART_ID);

        ASSERT_EQ(firstParentId, secondParentId);
        ASSERT_EQ(firstChildId, secondChildId);

        const CAnimationTrackKey firstTrack{
            firstChildId,
            AnimationProperty::PositionX
        };

        const CAnimationTrackKey secondTrack{
            secondChildId,
            AnimationProperty::PositionX
        };

        firstMotionEditor->AppearanceEditor().SetTexture(firstParentId, L"first-body.png");
        firstMotionEditor->AppearanceEditor().SetTexture(firstChildId, L"first-head.png");
        secondMotionEditor->AppearanceEditor().SetTexture(secondParentId, L"second-body.png");
        secondMotionEditor->AppearanceEditor().SetTexture(secondChildId, L"second-head.png");

        ASSERT_TRUE(firstMotionEditor->AnimationEditor().AddTrack(CAnimationTrack{ firstTrack }));
        ASSERT_TRUE(secondMotionEditor->AnimationEditor().AddTrack(CAnimationTrack{ secondTrack }));

        ASSERT_TRUE(firstSkeletonEditor.RemovePart(firstParentId));

        const auto* firstMotion = creature.FindMotionById(firstId);
        const auto* secondMotion = creature.FindMotionById(secondId);

        ASSERT_NE(firstMotion, nullptr);
        ASSERT_NE(secondMotion, nullptr);

        EXPECT_EQ(firstMotion->GetSkeleton().FindPartById(firstParentId), nullptr);
        EXPECT_EQ(firstMotion->GetSkeleton().FindPartById(firstChildId), nullptr);
        EXPECT_EQ(firstMotion->GetAppearance().FindByPartId(firstParentId), nullptr);
        EXPECT_EQ(firstMotion->GetAppearance().FindByPartId(firstChildId), nullptr);
        EXPECT_EQ(firstMotion->GetAnimation().FindAnimationTrack(firstTrack), nullptr);

        EXPECT_NE(secondMotion->GetSkeleton().FindPartById(secondParentId), nullptr);
        EXPECT_NE(secondMotion->GetSkeleton().FindPartById(secondChildId), nullptr);
        EXPECT_NE(secondMotion->GetAppearance().FindByPartId(secondParentId), nullptr);
        EXPECT_NE(secondMotion->GetAppearance().FindByPartId(secondChildId), nullptr);
        EXPECT_NE(secondMotion->GetAnimation().FindAnimationTrack(secondTrack), nullptr);
    }

    TEST(CreatureEditorTest, AddMotionWithIdUpdatesNextGeneratedIdAndRejectsMaxValue)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr MotionId explicitId = 100;

        ASSERT_EQ(editor.AddMotionWithId(explicitId, "Imported"), explicitId);
        EXPECT_EQ(editor.AddNewMotion("Generated"), explicitId + 1);

        CCreature maxIdCreature;
        CCreatureEditor maxIdEditor(maxIdCreature);

        EXPECT_EQ(
            maxIdEditor.AddMotionWithId(std::numeric_limits<MotionId>::max(), "Invalid"),
            INVALID_MOTION_ID);
    }

    TEST(CreatureEditorTest, RemoveMotionRemovesSelectedMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto firstId = editor.AddNewMotion("First");
        const auto secondId = editor.AddNewMotion("Second");

        ASSERT_NE(firstId, INVALID_MOTION_ID);
        ASSERT_NE(secondId, INVALID_MOTION_ID);

        ASSERT_TRUE(editor.RemoveMotion(firstId));

        EXPECT_EQ(creature.FindMotionById(firstId), nullptr);
        EXPECT_NE(creature.FindMotionById(secondId), nullptr);
        EXPECT_EQ(creature.GetMotions().size(), 1u);
    }
    // NOLINTEND(bugprone-unchecked-optional-access)
}
