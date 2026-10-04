#include "pch.h"

#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "MotionId.h"
#include "SkeletonEditor.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

namespace AnimationEditor
{
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
    class AnimationEditorTests : public ::testing::Test
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

    TEST_F(AnimationEditorTests, EmptyTrackCanBeAdded)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();

        const CAnimationTrackKey key
        {
            m_partId,
            AnimationProperty::PositionX
        };

        EXPECT_TRUE(editor.AddTrack(CAnimationTrack{ key }));
    }

    TEST_F(AnimationEditorTests, SetDurationAcceptsPositiveDuration)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(1.0f));

        const auto* motion = GetMotion();
        ASSERT_NE(motion, nullptr);

        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 1.0f);
    }

    TEST_F(AnimationEditorTests, SetDurationRejectsNegativeDuration)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        EXPECT_FALSE(editor.SetDuration(-1.0f));
    }

    TEST_F(AnimationEditorTests, SetDurationRejectsDurationShorterThanExistingKeyFrame)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(2.0f));

        const CAnimationTrackKey trackKey
        {
            m_partId,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));
        ASSERT_TRUE(editor.AddOrUpdateKeyFrame(trackKey, { 1.5f, 10.0f }));

        EXPECT_FALSE(editor.SetDuration(1.0f));

        const auto motion = GetMotion();
        ASSERT_NE(motion, nullptr);

        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 2.0f);
    }

    TEST_F(AnimationEditorTests, DuplicateTrackIsRejected)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();

        const CAnimationTrackKey trackKey
        {
            m_partId,
            AnimationProperty::Rotation
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));
        EXPECT_FALSE(editor.AddTrack(CAnimationTrack{ trackKey }));

        const auto motion = GetMotion();
        ASSERT_NE(motion, nullptr);

        EXPECT_EQ(motion->GetAnimation().GetAnimationTracks().size(), 1u);
    }

    TEST_F(AnimationEditorTests, KeyFramePastDurationIsRejected)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(1.0f));

        const CAnimationTrackKey trackKey
        {
            m_partId,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        EXPECT_FALSE(editor.AddOrUpdateKeyFrame(trackKey, { 1.1f, 10.0f }));
    }

    TEST_F(AnimationEditorTests, NegativeKeyFrameTimeIsRejected)
    {
        auto motionEditor = GetMotionEditor();
        ASSERT_TRUE(motionEditor.has_value());

        auto editor = motionEditor->AnimationEditor();
        ASSERT_TRUE(editor.SetDuration(1.0f));

        const CAnimationTrackKey trackKey
        {
            m_partId,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        EXPECT_FALSE(editor.AddOrUpdateKeyFrame(trackKey, { -0.1f, 10.0f }));
    }

    // NOLINTEND(bugprone-unchecked-optional-access)
}
