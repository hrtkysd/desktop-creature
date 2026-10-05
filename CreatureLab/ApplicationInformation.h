#pragma once

#include <string_view>

namespace Information
{
    namespace Product
    {
        inline constexpr std::string_view ProductName = "CreatureLab";
    }
    namespace Creature
    {
        inline constexpr std::string_view NewCreatureDefaultName = "Untitled ";
    }
    namespace File
    {
        inline constexpr std::string_view DefaultCreatureFileExtension = "creature";
        inline constexpr std::string_view DefaultCreatureFileName = "creature.creature";
    }
}
