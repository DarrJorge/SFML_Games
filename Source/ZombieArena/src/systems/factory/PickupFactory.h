//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../../core/GameTypes.h"

using namespace ZombieArena::Core::Types;

class SpawnConfig;
class Pickup;

class PickupFactory
{
public:
    explicit PickupFactory(const SpawnConfig& cfg);
    [[nodiscard]] std::unique_ptr<Pickup> make(PickupType type, const sf::Vector2f& pos) const;

private:
    const SpawnConfig& m_config;
};
