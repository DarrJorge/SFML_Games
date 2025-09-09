//
// Created by Oleksandr Pogorelov on 27.8.2025.
//

#pragma once

#include <cstdlib>
#include <vector>
#include <optional>
#include "Bullet.h"
#include "SFML/Graphics.hpp"

class Weapon;
class Bullet;
class Player;

class WeaponSystem
{
public:
    WeaponSystem();

    void update(float deltaTime);

    void startFire(const Player& owner);
    void stopFire(const Player& owner);

    std::vector<Bullet>& bullets() { return m_bullets; };
    [[nodiscard]] const std::vector<Bullet>& bullets() const { return m_bullets; };

    void setFireRate(float rate) { m_fireRate = rate; }
    void setClipSize(int32_t clip) { m_clipSize = clip; }
    void setReloadTime(float t)  { m_reloadTime = t; }

    [[nodiscard]] int32_t inClip() const { return m_bulletsInClip; }
    [[nodiscard]] int32_t clipSize() const { return m_clips; }

private:
    void tryShoot(const Player& owner);
    void reloadClip();

    bool m_firing{false};
    float m_fireRate{4.f};
    float m_cooldown{0.f};

    std::int32_t m_clipSize{6};
    std::int32_t m_bulletsInClip{6};
    std::int32_t m_clips{5};

    float m_reloadTime{1.5f};
    float m_reloadTimer{0.f};

    std::vector<Bullet> m_bullets;
    size_t m_nextBullet{0};

};
