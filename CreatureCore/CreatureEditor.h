#pragma once

#include "AnimationId.h"
#include "AnimationEditor.h"
#include "AppearanceEditor.h"
#include "ICreatureEditContext.h"
#include "PartId.h"
#include "SkeletonEditor.h"

#include <string>

namespace Creature
{
    class CCreature;

    namespace Animation
    {
        class CAnimation;
    }

    namespace Editor
    {
        class CCreatureEditor : private ICreatureEditContext
        {
        public:
            CCreatureEditor() = delete;
            explicit CCreatureEditor(Creature::CCreature& creature);
        public:
            CAnimationEditor& GetAnimationEditor();
            CSkeletonEditor& GetSkeletonEditor();
            CAppearanceEditor& GetAppearanceEditor();
        public:
            const Animation::CAnimation* FindAnimationById(Animation::AnimationId id) const;
            Animation::AnimationId AddNewAnimation(const std::string& strName);
            Animation::AnimationId AddAnimation(Animation::CAnimation&& animation);
            Animation::AnimationId AddAnimationWithId(Animation::AnimationId id, const std::string& name);
            bool RemoveAnimation(Animation::AnimationId id);

            const Creature::CCreature& GetCreature() const;
            void SetName(const std::string& strName);
        private:
            Animation::CAnimation* FindAnimationById(Animation::AnimationId id);
            bool RemovePart(PartId id) override;
        private:
            CCreature& m_creature;
            CAnimationEditor m_animationEditor;
            CSkeletonEditor m_skeletonEditor;
            CAppearanceEditor m_appearanceEditor;
        };

    }
}

