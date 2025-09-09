//
// Created by Oleksandr Pogorelov on 24.8.2025.
//

#pragma once

#include <numbers>
#include <string_view>

namespace ZombieArena::Core
{
    namespace Types
    {
        enum class GameState
        {
            START = 1,
            PAUSED,
            LEVELING_UP,
            PLAYING,
            GAME_OVER,
            COUNT
        };

        enum class WeaponType
        {
            RIFLE,
            LAUNCHER
        };

        enum class PickupType
        {
            HEALTH,
            AMMO,
            COUNT
        };

        enum class EEnemyType
        {
            BLOATER,
            CHASER,
            CRAWLER
        };

        struct PickupData
        {
            PickupType Type;
            double Value;
            float WaitSpawnTime;
            float TimeLife;

            PickupData(PickupType type, double value, float waitSpawnTime, float timeLife)
                : Type(type), Value(value), WaitSpawnTime(waitSpawnTime), TimeLife(timeLife) {}
        };

        struct AmmoData
        {
            std::uint32_t Bullets;
            std::uint32_t Clips;
            bool Infinite;
        };

        struct WeaponData
        {
            WeaponType Type;
            AmmoData Ammo;
        };

        struct EnemyData
        {
            float speed;
            float health;

            EnemyData(float inSpeed, float inHealth) : speed(inSpeed), health(inHealth){}
        };
    }

    namespace Constants
    {
        constexpr int TILE_SIZE = 50;
        constexpr int TILE_TYPES = 3;
        constexpr int VERTS_IN_TILE = 6;
        constexpr float PI = std::numbers::pi_v<float>;
        constexpr float TWO_PI = 2.f * std::numbers::pi;

    }

    namespace Texts
    {
        constexpr std::string_view PAUSED_TEXT("Press Enter to continue");
        constexpr std::string_view GAME_OVER_TEXT("Press Enter to play");
    }

    namespace Paths
    {
        constexpr std::string_view AMMO_ICON_PATH("graphics/ammo_icon.png");
        constexpr std::string_view PLAYER_AVATAR_PATH("graphics/player.png");
        constexpr std::string_view BACKGROUND_UI_PATH("graphics/background.png");
        constexpr std::string_view BACKGROUND_GAME_PATH("graphics/background_sheet.png");
        constexpr std::string_view CROSSHAIR_SPRITE_PATH("graphics/crosshair.png");

        constexpr std::string_view FONT_PATH("fonts/zombiecontrol.ttf");
    }

}
