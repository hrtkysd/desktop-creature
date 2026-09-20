#pragma once

#include "AnimationId.h"

#include <memory>
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

    class CCreature
    {
    public:
        CCreature();
        ~CCreature();

        CCreature(const CCreature&) = delete;
        CCreature& operator=(const CCreature&) = delete;

        CCreature(CCreature&&) noexcept;
        CCreature& operator=(CCreature&&) noexcept;
    public:
        const Genome& GetGenome() const;
        Genome& GetGenome();

        const CSkeleton& GetSkeleton() const;
        CSkeleton& GetSkeleton();

        const CAppearance& GetAppearance() const;
        CAppearance& GetAppearance();

        const std::string& GetName() const;
        void SetName(const std::string& strName);

        const std::vector<Animation::CAnimation>& GetAnimations() const;

        Animation::AnimationId AddAnimation(Animation::CAnimation&& animation);

        Animation::CAnimation& AddNewAnimation(const std::string& strName);

        Animation::CAnimation* FindAnimationById(Animation::AnimationId id);
        const Animation::CAnimation* FindAnimationById(Animation::AnimationId id) const;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
