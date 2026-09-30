#pragma once

#include "AnimationId.h"
#include "ICreatureEditContext.h"
#include "PartId.h"

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
        class CAnimationEditor;
        class CAppearanceEditor;
        class CSkeletonEditor;

        class CCreatureEditor : private ICreatureEditContext
        {
        public:
            CCreatureEditor() = delete;
            explicit CCreatureEditor(Creature::CCreature& creature);
        public:
            CAnimationEditor GetAnimationEditor();
            CSkeletonEditor GetSkeletonEditor();
            CAppearanceEditor GetAppearanceEditor();
        public:
            const Animation::CAnimation* FindAnimationById(Animation::AnimationId id) const;
            Animation::AnimationId AddNewAnimation(const std::string& strName);
            Animation::AnimationId AddAnimation(Animation::CAnimation&& animation);
            Animation::AnimationId AddAnimationWithId(Animation::AnimationId id, const std::string& name);
            bool RemoveAnimation(Animation::AnimationId id);

            const Creature::CCreature& GetCreature() const;
            void SetName(const std::string& strName);
        private:
            bool RemovePart(PartId id) override;
        private:
            CCreature& m_creature;
        };

    }
}

