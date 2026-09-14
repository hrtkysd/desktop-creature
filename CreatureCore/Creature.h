#pragma once

#include <memory>

namespace Creature
{
    struct Genome;
    struct Appearance;

    class CSkeleton;

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

        const Appearance& GetAppearance() const;
        Appearance& GetAppearance();

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
