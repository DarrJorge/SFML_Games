//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#pragma once
#include <vector>
#include "../world/World.h"
#include "../core/Events.h"

/// CombatSystem
/// -------------
/// Handles combat events:
///  - bullet hitting an enemy
///  - player picking up a pickup
///
/// Subscribes to events from EventBus,
/// stores them in internal queues,
/// and applies effects in update().
class CombatSystem
{
public:
    CombatSystem(World& world, EventBus& events);
    ~CombatSystem();

    void update(float deltaTime);

private:
    void processBulletHits();
    void processPickupTouches();
    void processEnemyHits();

private:
    World& m_world;
    EventBus& m_events;

    std::vector<BulletHitEnemyEvent> m_pendingBulletHits;
    std::vector<PlayerTouchedPickupEvent> m_pendingPickupTouches;
    std::vector<EnemyHitPlayerEvent> m_pendingEnemyHits;

    size_t m_subBulletHitId{0};
    size_t m_subPickupTouchId{0};
    size_t m_subEnemyHitId{0};

};
