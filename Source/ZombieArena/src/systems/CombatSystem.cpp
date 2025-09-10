//
// Created by Oleksandr Pogorelov.
//

#include "CombatSystem.h"
#include <algorithm>

namespace
{
    constexpr int BULLET_DAMAGE = 5;
    constexpr int SCORES_FOR_KILL = 10;
}

CombatSystem::CombatSystem(World& world, EventBus& events)
    : m_world(world), m_events(events)
{
    m_subBulletHitId = m_events.subscribe<BulletHitEnemyEvent>(
            [this](const BulletHitEnemyEvent& e) {
                m_pendingBulletHits.push_back(e);
            });

    m_subPickupTouchId = m_events.subscribe<PlayerTouchedPickupEvent>(
            [this](const PlayerTouchedPickupEvent& e) {
                m_pendingPickupTouches.push_back(e);
            });

    m_subEnemyHitId = m_events.subscribe<EnemyHitPlayerEvent>(
            [this](const EnemyHitPlayerEvent& e) {
                m_pendingEnemyHits.push_back(e);
            });
}

CombatSystem::~CombatSystem()
{
    m_events.unsubscribe<BulletHitEnemyEvent>(m_subBulletHitId);
    m_events.unsubscribe<PlayerTouchedPickupEvent>(m_subPickupTouchId);
    m_events.unsubscribe<EnemyHitPlayerEvent>(m_subEnemyHitId);
}

void CombatSystem::update(float deltaTime)
{
    processBulletHits();
    processPickupTouches();
    processEnemyHits();
}

void CombatSystem::processBulletHits()
{
    if (m_pendingBulletHits.empty()) return;

    auto& bullets = m_world.weapons().bullets();
    auto& enemies = m_world.enemies();

    for (const auto& hit : m_pendingBulletHits)
    {
        if (hit.bulletIdx >= bullets.size()) continue;
        if (hit.enemyIdx >= enemies.size()) continue;
        if (!enemies[hit.enemyIdx]) continue;

        auto& bullet = bullets[hit.bulletIdx];
        auto& enemy = *enemies[hit.enemyIdx];

        if (!bullet.isInFlight() || !enemy.isAlive())
            continue;

        bullet.stop();

        if (const bool killed = enemy.hit(BULLET_DAMAGE))
        {
            m_world.addScore(SCORES_FOR_KILL);

            if (std::none_of(enemies.begin(), enemies.end(),
                [](const auto& e){ return e && e->isAlive();}))
            {
                m_events.emit(GameStateChangedEvent{GameState::LEVELING_UP});
            }
        }
    }
    m_pendingBulletHits.clear();
}

void CombatSystem::processPickupTouches()
{
    if (m_pendingPickupTouches.empty()) return;

    auto& pickups = m_world.pickups();
    auto& player = m_world.player();

    for (const auto& touch : m_pendingPickupTouches)
    {
        if (touch.pickupIdx >= pickups.size()) continue;
        auto& pickup = pickups[touch.pickupIdx];
        if (!pickup || !pickup->isSpawned()) continue;

        if (player.tryApplyPickup(pickup.get()))
        {
            pickup->collect();
            m_events.emit(PickupCollectedEvent{touch.pickupIdx});
        }
    }

    m_pendingPickupTouches.clear();
}

void CombatSystem::processEnemyHits()
{
    if (m_pendingEnemyHits.empty()) return;

    auto& enemies = m_world.enemies();
    auto& player = m_world.player();

    for (const auto& hit : m_pendingEnemyHits)
    {
        if (hit.enemyIdx >= enemies.size()) continue;
        if (!enemies[hit.enemyIdx]) continue;

        auto& enemy = *enemies[hit.enemyIdx];
        if (!enemy.isAlive()) continue;

        if (player.hit(enemy.attack()))
        {
            m_events.emit(PlayerGetDamageEvent{});

            if (player.isDied())
            {
                m_events.emit(PlayerDiedEvent{});
            }
            break;
        }
    }
    m_pendingEnemyHits.clear();
}














