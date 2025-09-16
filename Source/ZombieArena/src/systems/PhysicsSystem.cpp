//
// Created by Oleksandr Pogorelov.
//

#include "PhysicsSystem.h"
#include "../utils/Utils.h"
#include <iostream>

using namespace ZombieArena::Utils;

PhysicsSystem::PhysicsSystem(World& world, EventBus& events)
    : m_world(world), m_events(events)
{
}

void PhysicsSystem::update(float deltaTime)
{
    auto& bullets = m_world.weapons().bullets();
    auto& enemies = m_world.enemies();

    const FloatRect& playerRect = m_world.player().getBounds();
    const Vector2f& playerCenter = Utils::rectCenter(playerRect);
    const float playerRadius = std::min(playerRect.size.x, playerRect.size.y);

    for (size_t i = 0; i < bullets.size(); ++i)
    {
        auto& bullet = bullets[i];
        if (!bullet.isInFlight()) continue;

        const auto& bAabb = bullet.getBounds();
        const auto& bulletCenter = Utils::rectCenter(bAabb);
        const float bulletRadius = std::min(bAabb.size.x, bAabb.size.y);

        for (size_t j = 0; j < enemies.size(); ++j)
        {
            auto& enemy = *enemies[j];
            if (!enemy.isAlive()) continue;
            const auto& enemyAabb = enemy.getPosition();
            const auto& enemyCenter = Utils::rectCenter(enemyAabb);
            const float enemyRadius = std::min(enemyAabb.size.x, enemyAabb.size.y);

            // check collision bullet with enemy
            if (Utils::circlesIntersect(bulletCenter, enemyCenter, bulletRadius, enemyRadius))
            {
                m_events.emit(BulletHitEnemyEvent{i, j});
            }

            // check collision enemy with player
            if (Utils::circlesIntersect(playerCenter, enemyCenter, playerRadius, enemyRadius))
            {
                m_events.emit(EnemyHitPlayerEvent{j});
            }
        }

        if (m_world.detectCollisionWalls(bullet.getPosition()))
        {
            bullet.stop();
        }
    }

    for (size_t j = 0; j < enemies.size(); ++j)
    {
        auto& enemy = *enemies[j];
        if (!enemy.isAlive()) continue;
        const auto& enemyAabb = enemy.getPosition();
        const auto& enemyCenter = Utils::rectCenter(enemyAabb);
        const float enemyRadius = std::min(enemyAabb.size.x, enemyAabb.size.y);

        // check collision enemy with player
        if (Utils::circlesIntersect(playerCenter, enemyCenter, playerRadius, enemyRadius))
        {
            m_events.emit(EnemyHitPlayerEvent{j});
        }
    }

    // check collision player with pickup
    for (size_t i = 0; i < m_world.pickups().size(); ++i)
    {
        auto& pickup = *m_world.pickups()[i];
        if (!pickup.isSpawned()) continue;

        const auto pickupCenter = Utils::rectCenter(pickup.getBounds());
        const float pickupRadius = std::min(pickup.getBounds().size.x, pickup.getBounds().size.y);

        if (Utils::circlesIntersect(playerCenter, pickupCenter, playerRadius, pickupRadius))
        {
            m_events.emit(PlayerTouchedPickupEvent{i});
        }
    }
}