//
// Created by Oleksandr Pogorelov.
//

#include "SpawnSystem.h"

#include <random>
#include <algorithm>
#include <memory>
#include "ResourceManager.h"

#include "../core/Events.h"
#include "../world/World.h"
#include "../utils/Utils.h"
#include "../world/pickups/HealthPickup.h"
#include "../world/pickups/AmmoPickup.h"

using namespace sf;
using namespace ZombieArena::Utils;

namespace
{
    constexpr int BASE_ENEMIES_COUNT = 7;
    constexpr int BASE_PICKUPS_COUNT = 1;
}


SpawnSystem::SpawnSystem(World& world, EventBus& events, float pickupRespawnSec)
    : m_world(world)
    , m_events(events)
    , m_pickupRespawnEvery(pickupRespawnSec)
    , m_pickupRespawnTimer(pickupRespawnSec)
{
    enemiesData.insert({EEnemyType::BLOATER, {3, 5}});
    enemiesData.insert({EEnemyType::CHASER, {5, 1}});
    enemiesData.insert({EEnemyType::CRAWLER, {1, 3}});

    m_texturesEnemies.insert({EEnemyType::BLOATER, &ResourceManager::getTexture("graphics/bloater.png")});
    m_texturesEnemies.insert({EEnemyType::CHASER, &ResourceManager::getTexture("graphics/chaser.png")});
    m_texturesEnemies.insert({EEnemyType::CRAWLER, &ResourceManager::getTexture("graphics/crawler.png")});


    pickupsData.insert({PickupType::HEALTH, {PickupType::HEALTH, 10, 5, 5}});
    pickupsData.insert({PickupType::AMMO, {PickupType::AMMO, 10, 5, 5}});

    m_texturesPickups.insert({PickupType::HEALTH, &ResourceManager::getTexture("graphics/health_pickup.png")});
    m_texturesPickups.insert({PickupType::AMMO, &ResourceManager::getTexture("graphics/ammo_pickup.png")});

    subscribe();
}

SpawnSystem::~SpawnSystem()
{
    unsubscribe();
}

void SpawnSystem::subscribe()
{
    m_stateChangedSubId = m_events.subscribe<GameStateChangedEvent>([this](const GameStateChangedEvent& e){
        if (e.newState == GameState::PLAYING)
        {
            if (isFinishedCurrentWave())
            {
                spawnNextWave(m_currentWaveIndex, BASE_ENEMIES_COUNT, BASE_PICKUPS_COUNT);
            }
        }
    });

    m_subPickupTouchId = m_events.subscribe<PickupCollectedEvent>([this](const PickupCollectedEvent& e){
        --m_activePickups;
    });
}

void SpawnSystem::unsubscribe()
{
    if (m_stateChangedSubId != 0)
    {
        m_events.unsubscribe<GameStateChangedEvent>(m_stateChangedSubId);
        m_events.unsubscribe<PickupCollectedEvent>(m_subPickupTouchId);
        m_stateChangedSubId = 0;
    }
}

void SpawnSystem::spawnNextWave(int waveIndex, int baseEnemies, int pickupsCount)
{
    const int enemies = std::max(1, baseEnemies + waveIndex * 3);
    spawnEnemies(enemies);
    spawnPickups(pickupsCount);
}

void SpawnSystem::update(float deltaTime)
{
    if (needSpawnMorePickups())
    {
        m_pickupRespawnTimer -= deltaTime;
        if (m_pickupRespawnTimer <= 0.f)
        {
            const int addPickups = getPickupCountForWave(m_currentWaveIndex) - m_world.pickups().size();
            spawnPickups(addPickups);
            m_pickupRespawnTimer = m_pickupRespawnEvery;
        }
    }
}

void SpawnSystem::spawnEnemies(int count)
{
    auto& rng = m_world.rng();
    const Vector2f playerPos = m_world.player().getCenter();
    const float MIN_DIST = 200.f;

    std::uniform_int_distribution<int> typeDist(0, static_cast<int>(EEnemyType::CRAWLER));
    std::uniform_int_distribution<int> modifierDist(1, 3);

    for (int i = 0; i < count; ++i)
    {
        auto type = static_cast<EEnemyType>(typeDist(rng));
        const float modifier = static_cast<float>(modifierDist(rng));
        const EnemyData& enemyData = enemiesData.at(type);
        const float speed = enemyData.speed * modifier;

        Sprite sprite(*m_texturesEnemies.at(type));

        Vector2f pos = randomPointFarFrom(playerPos, MIN_DIST, 32.f);
        auto enemy = std::make_unique<Enemy>(sprite, pos, speed, enemyData.health);
        enemy->setTarget(playerPos);
        m_world.enemies().push_back(std::move(enemy));
    }
}

void SpawnSystem::spawnPickups(int count)
{
    auto& rng = m_world.rng();

    std::uniform_int_distribution<int> typeDist(0, static_cast<int>(PickupType::COUNT) - 1);

    for (int i = 0; i < count; ++i)
    {
        Vector2f pos = randomPointInArena(34.f);

        const auto type = static_cast<PickupType>(typeDist(rng));
        const PickupData& value = pickupsData.at(type);

        std::unique_ptr<Pickup> pickup;

        switch (type)
        {
            case PickupType::AMMO:
                pickup = std::make_unique<AmmoPickup>(*m_texturesPickups.at(type), pos, value.Value);
                break;

            case PickupType::HEALTH:
                pickup = std::make_unique<HealthPickup>(*m_texturesPickups.at(type), pos, value.Value);
                break;
        }

        ++m_activePickups;

        m_world.pickups().push_back(std::move(pickup));
    }
}

Vector2f SpawnSystem::randomPointInArena(float margin) const
{
    const auto& A = m_world.arena();

    const float minX = static_cast<float>(A.position.x) + margin;
    const float minY = static_cast<float>(A.position.y) + margin;
    const float maxX = static_cast<float>(A.position.x + A.size.x) - margin;
    const float maxY = static_cast<float>(A.position.y + A.size.y) - margin;

    std::uniform_real_distribution<float> dx(minX, maxX);
    std::uniform_real_distribution<float> dy(minY, maxY);
    return { dx(m_world.rng()), dy(m_world.rng()) };
}

Vector2f SpawnSystem::randomPointFarFrom(const Vector2f& from, float minDistance, float margin) const
{
    for (int tries = 0; tries < 16; ++tries)
    {
        sf::Vector2f p = randomPointInArena(margin);
        if (Utils::length({p.x - from.x, p.y - from.y}) >= minDistance)
        {
            return p;
        }
    }
    return randomPointInArena(margin);
}

int SpawnSystem::getPickupCountForWave(int waveIndex) const
{
    return std::max(1, BASE_PICKUPS_COUNT * BASE_PICKUPS_COUNT);
}

bool SpawnSystem::needSpawnMorePickups() const
{
    return m_pickupRespawnEvery > 0.f && m_activePickups < getPickupCountForWave(m_currentWaveIndex);
}

bool SpawnSystem::isFinishedCurrentWave() const
{
    if (m_world.enemies().empty()) return true;

    auto& enemies = m_world.enemies();

    const bool anyAlive = std::any_of(enemies.begin(), enemies.end(),
        [](const auto& enemy) {
            return enemy && enemy->isAlive();
    });

    return !anyAlive;
}
