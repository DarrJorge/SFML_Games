//
// Created by Oleksandr Pogorelov on 2.9.2025.
//

#pragma once

#include <map>
#include <random>
#include <SFML/Graphics.hpp>
#include "core/GameTypes.h"


class Pickup;

using namespace ZombieArena::Core::Types;

class PickupSpawner
{
public:
    PickupSpawner(class ResourceManager& resourceManager);
    void spawn(int count, const sf::IntRect& arenaRect);

    std::vector<Pickup*>& pickups() { return m_pickups; }

    void draw(sf::RenderWindow& window);

private:
    std::vector<Pickup*> m_pickups;

    std::map<PickupType, PickupData> data;
    std::map<PickupType, const sf::Texture*> m_texturesPickups;

    std::mt19937 m_rand;
};