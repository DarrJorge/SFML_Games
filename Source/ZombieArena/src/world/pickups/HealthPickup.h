//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include "Pickup.h"

class HealthPickup : public Pickup
{
public:
    HealthPickup(const sf::Texture& texture, const sf::Vector2f& pos, int healAmount);
    bool applyTo(Player& player) override;

private:
    int m_heal;
};
