//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include "SFML/Graphics.hpp"
#include "src/core/GameTypes.h"

class Player;

using namespace ZombieArena::Core::Types;

class Pickup
{
public:
    Pickup(const sf::Texture& texture, const sf::Vector2f& pos);
    virtual ~Pickup() = default;

    void spawn(const sf::Vector2f& spawnPosition);
    virtual bool applyTo(Player& player) = 0;

    bool isSpawned() const;
    void collect();

    sf::FloatRect getBounds() const;
    sf::Sprite& getSprite();

private:
    sf::Sprite m_sprite;
    bool m_spawned{false};
};
