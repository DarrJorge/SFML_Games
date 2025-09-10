//
// Created by Oleksandr Pogorelov
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
