//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

class World;
class EventBus;
class SpawnConfig;
class EnemyFactory;
class PickupFactory;


class SpawnSystem
{
public:
    explicit SpawnSystem(World& world, EventBus& events, const SpawnConfig& cfg, const EnemyFactory& enemyFactory,
                         const PickupFactory& pickupFactory, float pickupRespawnSec = 0.f);
    ~SpawnSystem();

    void spawnNextWave(int waveIndex, int baseEnemies, int pickupsCount);
    void update(float deltaTime);

private:
    void spawnEnemies(int count);
    void spawnPickups(int count);

    [[nodiscard]] sf::Vector2f randomPointInArena(float margin = 32.f) const;
    [[nodiscard]] sf::Vector2f randomPointFarFrom(const sf::Vector2f& from, float minDistance, float margin = 32.f) const;

    void subscribe();
    void unsubscribe();
    [[nodiscard]] int getPickupCountForWave() const;
    [[nodiscard]] bool needSpawnMorePickups() const;

private:
    World& m_world;
    EventBus& m_events;

    const SpawnConfig& m_config;
    const EnemyFactory& m_enemyFactory;
    const PickupFactory& m_pickupFactory;

    float m_pickupRespawnEvery{0.f};
    float m_pickupRespawnTimer{0.f};

    size_t m_subPickupTouchId{0};
    int m_currentWaveIndex{1};

    size_t m_activePickups{0};
};
