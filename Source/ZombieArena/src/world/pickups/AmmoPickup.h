//
// Created by Oleksandr Pogorelov on 6.9.2025.
//

#pragma once
#include "Pickup.h"
#include "../WeaponSystem.h"

class Player;

class AmmoPickup : public Pickup
{
public:
    AmmoPickup(const sf::Texture& tex, const sf::Vector2f& pos, int bullets);
    bool applyTo(Player& player) override;

private:
    int m_ammo;
};
