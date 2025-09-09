//
// Created by Oleksandr Pogorelov on 24.8.2025.
//

#include "EnemySpawner.h"
#include "src/world/Enemy.h"
#include "ResourceManager.h"

using namespace sf;

namespace
{
    const int MAX_VARRIANCE = 30;
    const int OFFSET = 101 - MAX_VARRIANCE;
}

EnemySpawner::EnemySpawner(ResourceManager& resourceManager) : m_rand(std::random_device{}())
{
    data.insert({EEnemyType::BLOATER, {3, 5}});
    data.insert({EEnemyType::CHASER, {5, 1}});
    data.insert({EEnemyType::CRAWLER, {1, 3}});

    m_texturesEnemies.insert({EEnemyType::BLOATER, &resourceManager.getTexture("graphics/bloater.png")});
    m_texturesEnemies.insert({EEnemyType::CHASER, &resourceManager.getTexture("graphics/chaser.png")});
    m_texturesEnemies.insert({EEnemyType::CRAWLER, &resourceManager.getTexture("graphics/crawler.png")});
}

void EnemySpawner::spawn(int count, const sf::IntRect& arenaRect)
{
    m_enemies.reserve(count);

    int maxX = arenaRect.size.x - 20;
    int minX = arenaRect.position.x + 20;
    int maxY = arenaRect.size.y - 20;
    int minY = arenaRect.position.y + 20;

    std::uniform_int_distribution<int> sideDist(0, 3);
    std::uniform_int_distribution<int> typeDist(0, static_cast<int>(EEnemyType::CRAWLER));
    std::uniform_int_distribution<int> modifierDist(OFFSET, OFFSET + MAX_VARRIANCE - 1);
    std::uniform_int_distribution<int> posXDist(minX, maxX);
    std::uniform_int_distribution<int> posYDist(minY, maxY);

    for (int i = 0; i < count; ++i)
    {
        int side = sideDist(m_rand);
        int x = 0, y = 0;

        switch (side)
        {
            case 0: x = minX; y = posYDist(m_rand); break;  // left
            case 1: x = maxX; y = posYDist(m_rand); break; // right
            case 2: x = posXDist(m_rand); y = minY; break; // top
            case 3: x = posXDist(m_rand); y = maxY; break; // bottom
        }

        auto type = static_cast<EEnemyType>(typeDist(m_rand));
        const float modifier = static_cast<float>(modifierDist(m_rand));
        const EnemyData& value = data.at(type);
        const float speed = value.speed * modifier;

        sf::Sprite sprite(*m_texturesEnemies.at(type));
        const Vector2f spawnPosition = Vector2f(static_cast<float>(x), static_cast<float>(y));
        m_enemies.push_back(new Enemy(sprite, spawnPosition, speed, value.health));
    }
}