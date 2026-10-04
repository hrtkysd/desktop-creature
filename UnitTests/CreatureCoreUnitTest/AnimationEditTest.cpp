#include "pch.h"
#include "Animation.h"
#include "MotionId.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Creature.h"
#include "CreatureEditor.h"

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
        auto editor = m_creatureEditor.MotionEditor(m_motionId);
        const CAnimationTrackKey key
        {
            1,
            AnimationProperty::PositionX
        };

        CAnimationTrack track{ key };

        EXPECT_TRUE(
            editor.AddTrack(
                m_motionId,
                std::move(track)));
    }

    TEST_F(MotionEditorTests, SetDurationAcceptsPositiveDuration)
    {
        auto editor = m_creatureEditor.GetAnimationEditor();

        ASSERT_TRUE(editor.SetDuration(m_animationId, 1.0f));

        const auto animation
            = m_creatureEditor.FindAnimationById(m_animationId);

        ASSERT_NE(animation, nullptr);
        EXPECT_FLOAT_EQ(animation->GetDuration(), 1.0f);
    }

    TEST_F(MotionEditorTests, SetDurationRejectsNegativeDuration)
    {
        auto editor = m_creatureEditor.GetAnimationEditor();
        EXPECT_FALSE(editor.SetDuration(m_animationId, -1.0f));
    }

    TEST_F(
        MotionEditorTests,
        SetDurationRejectsDurationShorterThanExistingKeyFrame)
    {
        auto editor = m_creatureEditor.GetAnimationEditor();

        ASSERT_TRUE(editor.SetDuration(m_animationId, 2.0f));

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(
            editor.AddTrack(
                m_animationId,
                CAnimationTrack{ trackKey }));

        ASSERT_TRUE(
            editor.AddOrUpdateKeyFrame(
                m_animationId,
                trackKey,
                { 1.5f, 10.0f }));

        EXPECT_FALSE(
            editor.SetDuration(m_animationId, 1.0f));

        const auto animation =
            m_creatureEditor.FindAnimationById(m_animationId);

        ASSERT_NE(animation, nullptr);

        EXPECT_FLOAT_EQ(animation->GetDuration(), 2.0f);
    }

    TEST_F(MotionEditorTests, DuplicateTrackIsRejected)
    {
        auto editor = m_creatureEditor.GetAnimationEditor();

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::Rotation
        };

        ASSERT_TRUE(
            editor.AddTrack(
                m_animationId,
                CAnimationTrack{ trackKey }));

        EXPECT_FALSE(
            editor.AddTrack(
                m_animationId,
                CAnimationTrack{ trackKey }));

        const auto animation =
            m_creatureEditor.FindAnimationById(m_animationId);

        ASSERT_NE(animation, nullptr);
        EXPECT_EQ(animation->GetAnimationTracks().size(), 1u);
    }

    TEST_F(MotionEditorTests, KeyFramePastDurationIsRejected)
    {
        auto editor = m_creatureEditor.GetAnimationEditor();

        ASSERT_TRUE(editor.SetDuration(m_animationId, 1.0f));

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(
            editor.AddTrack(
                m_animationId,
                CAnimationTrack{ trackKey }));

        EXPECT_FALSE(
            editor.AddOrUpdateKeyFrame(
                m_animationId,
                trackKey,
                { 1.1f, 10.0f }));
    }

    TEST_F(MotionEditorTests, NegativeKeyFrameTimeIsRejected)
    {
        auto editor = m_creatureEditor.GetAnimationEditor();

        ASSERT_TRUE(editor.SetDuration(m_animationId, 1.0f));

        const CAnimationTrackKey trackKey
        {
            1,
            AnimationProperty::PositionX
        };

        ASSERT_TRUE(
            editor.AddTrack(
                m_animationId,
                CAnimationTrack{ trackKey }));

        EXPECT_FALSE(
            editor.AddOrUpdateKeyFrame(
                m_animationId,
                trackKey,
                { -0.1f, 10.0f }));
    }
}
