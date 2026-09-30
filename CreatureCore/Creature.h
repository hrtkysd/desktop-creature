#pragma once

#include "AnimationId.h"

#include <memory>
#include <string>
#include <vector>

namespace Creature
{
    struct Genome;

    class CAppearance;
    class CSkeleton;

    namespace Animation
    {
        class CAnimation;
    }

    namespace Editor
    {
        class CCreatureEditor;
    }

    namespace IO
    {
        class CCreatureIO;
    }

    class CCreature
    {
        friend class IO::CCreatureIO;
        friend class Creature::Editor::CCreatureEditor;
    public:
        CCreature();
        ~CCreature();

        CCreature(const CCreature&) = delete;
        CCreature& operator=(const CCreature&) = delete;

        CCreature(CCreature&&) noexcept;
        CCreature& operator=(CCreature&&) noexcept;
    public:
        CCreature Clone() const;
        const Genome& GetGenome() const;
        const CSkeleton& GetSkeleton() const;
        const CAppearance& GetAppearance() const;

        const std::string& GetName() const;

        const std::vector<Animation::CAnimation>& GetAnimations() const;
        const Animation::CAnimation* FindAnimationById(Animation::AnimationId id) const;
    private:
        Genome& MutableGenome();
        CSkeleton& MutableSkeleton();
        CAppearance& MutableAppearance();
        std::vector<Animation::CAnimation>& MutableAnimations();
    private:
        Animation::AnimationId AddAnimation(Animation::CAnimation&& animation);
        bool RemoveAnimation(Animation::AnimationId id);

        Animation::AnimationId AddNewAnimation(const std::string& strName);

        void SetName(const std::string& strName);
        Animation::AnimationId AddAnimationWithId(Animation::AnimationId id, const std::string& name);
        Animation::AnimationId GenerateAnimationId();
        void UpdateNextAnimationId(Animation::AnimationId id);
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
