#pragma once

#include "MotionId.h"
#include "MotionEditor.h"

#include <optional>
#include <string>

namespace Creature
{
    class CCreature;


    namespace Editor
    {
        class CCreatureEditor
        {
        public:
            CCreatureEditor() = delete;
            explicit CCreatureEditor(Creature::CCreature& creature);
        public:
            std::optional<CMotionEditor> MotionEditor(MotionId id);
        public:
            const CMotion* FindMotionById(MotionId id) const;
            MotionId AddNewMotion(const std::string& strName);
            MotionId AddMotion(CMotion&& motion);
            MotionId AddMotionWithId(MotionId id, const std::string& strName);
            bool RemoveMotion(MotionId id);

            void SwapCreature(CCreature&& creature);

            const Creature::CCreature& GetCreature() const;
            void SetName(const std::string& strName);
        private:
            CCreature& m_creature;
        };
    }
}

