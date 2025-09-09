//
// Created by Oleksandr Pogorelov on 6.9.2025.
//

#include "AmmoPickup.h"

using namespace sf;

AmmoPickup::AmmoPickup(const Texture& tex, const Vector2f& pos, int bullets)
        : Pickup(tex, pos), m_ammo(bullets)
{
}

bool AmmoPickup::applyTo(Player& player)
{
    // предположим, что Player умеет давать доступ к WeaponSystem
    // или можно вызвать player.addAmmo(m_ammo);
    // тут зависит от твоей архитектуры
    return true;
}