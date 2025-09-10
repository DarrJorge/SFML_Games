//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vector>
#include <cstddef>
#include <map>
#include "../core/GameTypes.h"

class World;
class EventBus;
class Enemy;
class Pickup;

using namespace ZombieArena::Core::Types;


class SpawnSystem
{
public:
    explicit SpawnSystem(World& world, EventBus& events, float pickupRespawnSec = 0.f);
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
    [[nodiscard]] int getPickupCountForWave(int waveIndex) const;
    [[nodiscard]] bool needSpawnMorePickups() const;
    bool isFinishedCurrentWave() const;

private:
    World& m_world;
    EventBus& m_events;

    float m_pickupRespawnEvery{0.f};
    float m_pickupRespawnTimer{0.f};

    std::map<EEnemyType, EnemyData> enemiesData;
    std::map<EEnemyType, const sf::Texture*> m_texturesEnemies;

    std::map<PickupType, PickupData> pickupsData;
    std::map<PickupType, const sf::Texture*> m_texturesPickups;

    std::size_t m_stateChangedSubId{0};
    size_t m_subPickupTouchId{0};
    size_t m_subGameOverId{0};
    int m_currentWaveIndex{1};

    size_t m_activePickups{0};
};
