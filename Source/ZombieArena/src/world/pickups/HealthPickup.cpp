//
// Created by Oleksandr Pogorelov on 6.9.2025.
//

#include "HealthPickup.h"
#include "../Player.h"

using namespace sf;

HealthPickup::HealthPickup(const Texture& tex, const Vector2f& pos, int healAmount)
    : Pickup(tex, pos), m_heal(healAmount)
{
}


bool HealthPickup::applyTo(Player& player)
{
    return player.heal(m_heal);
}