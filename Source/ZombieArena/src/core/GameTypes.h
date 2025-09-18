//
// Created by Oleksandr Pogorelov.
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
            WIN,
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
            CRAWLER,
            COUNT
        };

        enum class SoundType
        {
            ENEMY_HIT,
            PLAYER_HIT,
            PICKUP,
            SHOOT,
            RELOAD_WEAPON,
            POWERUP,
            NO_AMMO,
            COUNT
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
            int32_t damage;

            EnemyData(float inSpeed, float inHealth, int32_t inDamage)
                : speed(inSpeed), health(inHealth), damage(inDamage) {}
        };
    }

    namespace Constants
    {
        constexpr int TILE_SIZE = 50;
        constexpr int TILE_TYPES = 3;
        constexpr int VERTS_IN_TILE = 6;
        constexpr float PI = std::numbers::pi_v<float>;
        constexpr float TWO_PI = 2.f * std::numbers::pi;
        constexpr int BASE_ENEMIES_COUNT = 7;
        constexpr int BASE_PICKUPS_COUNT = 1;
    }

    namespace Paths
    {
        constexpr std::string_view AMMO_ICON_PATH("graphics/ammo_icon.png");
        constexpr std::string_view PLAYER_AVATAR_PATH("graphics/player.png");
        constexpr std::string_view BACKGROUND_UI_PATH("graphics/splashscreen.png");
        constexpr std::string_view BACKGROUND_GAME_PATH("graphics/background_sheet.png");
        constexpr std::string_view CROSSHAIR_SPRITE_PATH("graphics/crosshair.png");

        constexpr std::string_view FONT_PATH("fonts/zombiecontrol.ttf");

        namespace Enemies
        {
            constexpr std::string_view ENEMY_BLOATER("graphics/bloater.png");
            constexpr std::string_view ENEMY_CHASER("graphics/chaser.png");
            constexpr std::string_view ENEMY_CRAWLER("graphics/crawler.png");
        }

        namespace Pickups
        {
            constexpr std::string_view AMMO_PICKUP("graphics/ammo_pickup.png");
            constexpr std::string_view HEALTH_PICKUP("graphics/health_pickup.png");
        }
    }
}
