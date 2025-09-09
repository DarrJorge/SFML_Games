//
// Created by Oleksandr Pogorelov on 27.8.2025.
//

#include "WeaponSystem.h"
#include "Player.h"
#include "../utils/Utils.h"

using namespace sf;
using namespace ZombieArena::Utils;

WeaponSystem::WeaponSystem()
{
    m_bullets.resize(100);
}

void WeaponSystem::update(float deltaTime)
{
    // update timer reloading clip
    if (m_reloadTimer > 0.f)
    {
        m_reloadTimer -= deltaTime;
        if (m_reloadTimer <= 0.f)
        {
            reloadClip();
        }
    }

    // update cooldown firing
    if (m_cooldown > 0.f)
    {
        m_cooldown -= deltaTime;
    }

    for (auto& b : m_bullets)
    {
        if (b.isInFlight())
        {
            b.update(deltaTime);
        }
    }
}

void WeaponSystem::reloadClip()
{
    if (m_clips > 0)
    {
        m_bulletsInClip = m_clipSize;
        m_clips = std::clamp(m_clips - 1, 0, m_clips);
    }
}

void WeaponSystem::startFire(const Player& owner)
{
    if (m_reloadTimer > 0.f) return;
    m_firing = true;
    tryShoot(owner);
}

void WeaponSystem::stopFire(const Player& owner)
{
    m_firing = false;
}

void WeaponSystem::tryShoot(const Player& owner)
{
    if (m_cooldown > 0.f) return;
    if (m_bulletsInClip <= 0)
    {
        m_reloadTimer = m_reloadTime;
        return;
    }

    auto& bullet = m_bullets[m_nextBullet];
    m_nextBullet = (m_nextBullet + 1) % m_bullets.size();

    const auto playerPos = owner.getCenter();
    const auto aimPos = owner.getAimPosition();
    const auto spawnBulletPos = playerPos + Utils::Normalize2D(aimPos - playerPos) * 10.f;
    bullet.shoot(spawnBulletPos, aimPos);

    m_bulletsInClip--;
    m_cooldown = 1.f / m_fireRate;
}