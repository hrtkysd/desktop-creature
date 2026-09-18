#pragma once

#include <cstdint>
#include <memory>

namespace Creature
{
    struct Genome;

    class CAppearance;
    class CSkeleton;

    namespace Animation
    {
        class CAnimation;
        using AnimationId = std::uint32_t;
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

        Animation::AnimationId AddAnimation(Animation::CAnimation&& animation);

        Animation::CAnimation* FindAnimationById(Animation::AnimationId id);
        const Animation::CAnimation* FindAnimationById(Animation::AnimationId id) const;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
