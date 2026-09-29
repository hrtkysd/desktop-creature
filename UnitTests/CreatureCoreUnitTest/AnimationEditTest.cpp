#include "pch.h"
#include "Animation.h"
#include "AnimationId.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Creature.h"
#include "CreatureEditor.h"


using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

namespace AnimationEditor
{
    class AnimationEditorTests : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            m_animationId = m_creatureEditor.AddNewAnimation("idle");

            ASSERT_NE(
                m_animationId,
                INVALID_ANIMATION_ID);
        }

    protected:
        CCreature m_creature;
        CCreatureEditor m_creatureEditor{ m_creature };

        AnimationId m_animationId = INVALID_ANIMATION_ID;
    };

    TEST_F(AnimationEditorTests, EmptyTrackCanBeAdded)
    {
        auto& editor =
            m_creatureEditor.GetAnimationEditor();

        const CAnimationTrackKey key{
            1,
            AnimationProperty::PositionX
        };

        CAnimationTrack track{ key };

        EXPECT_TRUE(
            editor.AddTrack(
                m_animationId,
                std::move(track)));
    }

    TEST_F(AnimationEditorTests, SetDurationAcceptsPositiveDuration)
    {
        auto& editor = m_creatureEditor.GetAnimationEditor();

        ASSERT_TRUE(editor.SetDuration(m_animationId, 1.0f));

        const auto animation
            = m_creatureEditor.FindReadonlyAnimationById(m_animationId);

        ASSERT_NE(animation, nullptr);
        EXPECT_FLOAT_EQ(animation->GetDuration(), 1.0f);
    }

    TEST_F(AnimationEditorTests, SetDurationRejectsNegativeDuration)
    {
        auto editor = m_creatureEditor.GetAnimationEditor();
        EXPECT_FALSE(editor.SetDuration(m_animationId, -1.0f));
    }

    TEST_F(
        AnimationEditorTests,
        SetDurationRejectsDurationShorterThanExistingKeyFrame)
    {
        auto& editor = m_creatureEditor.GetAnimationEditor();

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
            m_creatureEditor.FindReadonlyAnimationById(m_animationId);

        ASSERT_NE(animation, nullptr);

        EXPECT_FLOAT_EQ(animation->GetDuration(), 2.0f);
    }

    TEST_F(AnimationEditorTests, DuplicateTrackIsRejected)
    {
        auto& editor = m_creatureEditor.GetAnimationEditor();

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
            m_creatureEditor.FindReadonlyAnimationById(m_animationId);

        ASSERT_NE(animation, nullptr);
        EXPECT_EQ(animation->GetAnimationTracks().size(), 1u);
    }

    TEST_F(AnimationEditorTests, KeyFramePastDurationIsRejected)
    {
        auto& editor = m_creatureEditor.GetAnimationEditor();

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

    TEST_F(AnimationEditorTests, NegativeKeyFrameTimeIsRejected)
    {
        auto& editor = m_creatureEditor.GetAnimationEditor();

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
