//
// Created by Oleksandr Pogorelov on 24.8.2025.
//

#pragma once

#include <vector>
#include <map>
#include <random>
#include <SFML/Graphics.hpp>
#include "core/GameTypes.h"

class Enemy;

using namespace ZombieArena::Core::Types;

class EnemySpawner
{
public:
    EnemySpawner(class ResourceManager& resourceManager);
    void spawn(int count, const sf::IntRect& arenaRect);

    std::vector<Enemy*>& enemies() { return m_enemies; }

private:
    std::vector<Enemy*> m_enemies;
    std::map<EEnemyType, EnemyData> data;
    std::map<EEnemyType, const sf::Texture*> m_texturesEnemies;;

    std::mt19937 m_rand;

};
