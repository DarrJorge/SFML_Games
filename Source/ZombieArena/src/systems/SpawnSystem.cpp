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
using namespace ZombieArena::Core::Constants;

namespace
{
    constexpr float MIN_DIST = 200.f;
    constexpr float MIN_DIST_SQUARE = MIN_DIST * MIN_DIST;
}


SpawnSystem::SpawnSystem(World& world, EventBus& events, float pickupRespawnSec)
    : m_world(world)
    , m_events(events)
    , m_pickupRespawnEvery(pickupRespawnSec)
    , m_pickupRespawnTimer(pickupRespawnSec)
{
    enemiesData.insert({EEnemyType::BLOATER, {2, 5, 3}});
    enemiesData.insert({EEnemyType::CHASER, {5, 1, 1}});
    enemiesData.insert({EEnemyType::CRAWLER, {3, 3, 2}});

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
    m_subPickupTouchId = m_events.subscribe<PickupCollectedEvent>([this](const PickupCollectedEvent& e){
        --m_activePickups;
    });
}

void SpawnSystem::unsubscribe()
{
    if (m_subPickupTouchId != 0) m_events.unsubscribe<PickupCollectedEvent>(m_subPickupTouchId);
}

void SpawnSystem::spawnNextWave(int waveIndex, int baseEnemies, int pickupsCount)
{
    m_currentWaveIndex = waveIndex;
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
            const int deficit = std::max(0, static_cast<int>(getPickupCountForWave() - m_activePickups));
            if (deficit > 0) spawnPickups(deficit);
            m_pickupRespawnTimer = m_pickupRespawnEvery;
        }
    }
}

void SpawnSystem::spawnEnemies(int count)
{
    auto& rng = m_world.rng();
    const Vector2f playerPos = m_world.player().getCenter();

    std::uniform_int_distribution<int> typeDist(0, static_cast<int>(EEnemyType::COUNT) - 1);
    std::uniform_int_distribution<int> modifierDist(1, 3);

    for (int i = 0; i < count; ++i)
    {
        auto type = static_cast<EEnemyType>(typeDist(rng));
        const float modifier = static_cast<float>(modifierDist(rng));
        const EnemyData& enemyData = enemiesData.at(type);
        const float speed = enemyData.speed * modifier;

        Vector2f pos = randomPointFarFrom(playerPos, MIN_DIST_SQUARE, TILE_SIZE);
        auto enemy = std::make_unique<Enemy>(Sprite{*m_texturesEnemies.at(type)}, pos, enemyData);
        if (!enemy) continue;

        enemy->setSpeed(speed);
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
        Vector2f pos = randomPointInArena(TILE_SIZE);

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
        if (Utils::squaredLength({p.x - from.x, p.y - from.y}) >= minDistance)
        {
            return p;
        }
    }
    return randomPointInArena(margin);
}

int SpawnSystem::getPickupCountForWave() const
{
    return std::max(1, BASE_PICKUPS_COUNT * m_currentWaveIndex);
}

bool SpawnSystem::needSpawnMorePickups() const
{
    return m_pickupRespawnEvery > 0.f && m_activePickups < getPickupCountForWave();
}
