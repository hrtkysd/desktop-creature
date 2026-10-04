#pragma once

#include "MotionId.h"

#include <memory>
#include <string>
#include <vector>

namespace Creature
{
    struct Genome;
    class CMotion;

    namespace Editor
    {
        class CCreatureEditor;
    }

    class CCreature
    {
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

        const std::string& GetName() const;

        const std::vector<CMotion>& GetMotions() const;
        const CMotion* FindMotionById(MotionId id) const;
    private:
        Genome& MutableGenome();
        std::vector<CMotion>& MutableMotions();
        CMotion* FindMutableMotionById(MotionId id);
    private:

        void SetName(const std::string& strName);

        MotionId AddMotion(CMotion&& motion);
        bool RemoveMotion(MotionId id);
        MotionId AddNewMotion(const std::string& strName);
        MotionId AddMotionWithId(MotionId id, const std::string& strName);
        MotionId GenerateMotionId();
        void UpdateNextMotionId(MotionId id);

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
