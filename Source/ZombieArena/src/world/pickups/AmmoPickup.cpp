//
// Created by Oleksandr Pogorelov.
//

#include "AmmoPickup.h"

using namespace sf;

AmmoPickup::AmmoPickup(const Texture& tex, const Vector2f& pos, int bullets)
        : Pickup(tex, pos), m_ammo(bullets)
{
}

bool AmmoPickup::applyTo(Player& player)
{
    // TODO @now is true
    return true;
}