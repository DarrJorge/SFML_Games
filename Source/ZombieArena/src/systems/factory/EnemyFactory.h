//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../../core/GameTypes.h"

using namespace ZombieArena::Core::Types;


class Enemy;
class SpawnConfig;

class EnemyFactory
{
public:
    EnemyFactory(const SpawnConfig& config);
    [[nodiscard]] std::unique_ptr<Enemy> make(EEnemyType type, const sf::Vector2f& pos) const;

private:
    [[nodiscard]] sf::Sprite makeSprite(EEnemyType type) const;

    const SpawnConfig& m_config;
};
