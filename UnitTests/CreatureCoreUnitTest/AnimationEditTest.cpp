#include "pch.h"
#include "AnimationEditor.h"
#include "Animation.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "MotionEditor.h"
#include "MotionId.h"
#include "AnimationTrackKey.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

namespace MotionEditor
{
    class MotionEditorTests : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            m_motionId = m_creatureEditor.AddNewMotion("idle");
            ASSERT_NE(m_motionId, INVALID_MOTION_ID);
        }

    protected:
        CCreature m_creature;
        CCreatureEditor m_creatureEditor{ m_creature };

        MotionId m_motionId = INVALID_MOTION_ID;
    };

    TEST_F(MotionEditorTests, EmptyTrackCanBeAdded)
    {
        auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->AnimationEditor();

        const CAnimationTrackKey key
        {
            1,
            AnimationProperty::PositionX
        };

        CAnimationTrack track{ key };

        EXPECT_TRUE(editor.AddTrack(std::move(track)));
    }

    TEST_F(MotionEditorTests, SetDurationAcceptsPositiveDuration)
    {
        auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->AnimationEditor();

        ASSERT_TRUE(editor.SetDuration(1.0f));

        const auto motion = m_creatureEditor.FindMotionById(m_motionId);

        ASSERT_NE(motion, nullptr);
        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 1.0f);
    }

    TEST_F(MotionEditorTests, SetDurationRejectsNegativeDuration)
    {
        auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->AnimationEditor();

        EXPECT_FALSE(editor.SetDuration(-1.0f));
    }

    TEST_F(
        MotionEditorTests,
        SetDurationRejectsDurationShorterThanExistingKeyFrame)
    {
        auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->AnimationEditor();

        ASSERT_TRUE(editor.SetDuration(2.0f));

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        ASSERT_TRUE(
            editor.AddOrUpdateKeyFrame(
                trackKey,
                { 1.5f, 10.0f }));

        EXPECT_FALSE(editor.SetDuration(1.0f));

        const auto motion = m_creatureEditor.FindMotionById(m_motionId);

        ASSERT_NE(motion, nullptr);

        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 2.0f);
    }

    TEST_F(MotionEditorTests, DuplicateTrackIsRejected)
    {
        auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->AnimationEditor();

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::Rotation
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        EXPECT_FALSE(editor.AddTrack(CAnimationTrack{ trackKey }));

        const auto motion = m_creatureEditor.FindMotionById(m_motionId);

        ASSERT_NE(motion, nullptr);
        EXPECT_EQ(motion->GetAnimation().GetAnimationTracks().size(), 1u);
    }

    TEST_F(MotionEditorTests, KeyFramePastDurationIsRejected)
    {
        auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->AnimationEditor();

        ASSERT_TRUE(editor.SetDuration(1.0f));

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        EXPECT_FALSE(
            editor.AddOrUpdateKeyFrame(
                trackKey,
                { 1.1f, 10.0f }));
    }

    TEST_F(MotionEditorTests, NegativeKeyFrameTimeIsRejected)
    {
        auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->AnimationEditor();

        ASSERT_TRUE(editor.SetDuration(1.0f));

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ trackKey }));

        EXPECT_FALSE(
            editor.AddOrUpdateKeyFrame(
                trackKey,
                { -0.1f, 10.0f }));
    }
}
