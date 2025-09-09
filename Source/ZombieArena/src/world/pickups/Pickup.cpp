//
// Created by Oleksandr Pogorelov on 2.9.2025.
//

#include "Pickup.h"

using namespace sf;
using namespace ZombieArena::Core::Types;

Pickup::Pickup(const Texture& texture, const Vector2f& pos)
    : m_sprite(texture)
{
    m_sprite.setOrigin(Vector2f{texture.getSize().x * 0.5f, texture.getSize().y * 0.5f});
    spawn(pos);
}

void Pickup::spawn(const Vector2f& spawnPosition)
{
    m_spawned = true;
    m_sprite.setPosition(spawnPosition);
}

sf::FloatRect Pickup::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

sf::Sprite& Pickup::getSprite()
{
    return m_sprite;
}

bool Pickup::isSpawned() const
{
    return m_spawned;
}

void Pickup::collect()
{
    m_spawned = false;
}

