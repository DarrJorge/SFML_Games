//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include "SFML/Graphics.hpp"
#include "../core/GameTypes.h"

using namespace ZombieArena::Core::Types;

class Enemy
{
public:
    Enemy(sf::Sprite sprite, const sf::Vector2f& position, const EnemyData& data);

    bool hit(float amount);
    void update(float elapsedTime);
    void setTarget(const sf::Vector2f& target);
    void setSpeed(float speed) { m_speed = speed; }
    int32_t attack() const;

    sf::FloatRect getPosition() const;
    bool isAlive() const { return m_alive; }
    sf::Sprite getSprite() const { return m_sprite; }

private:

    sf::Vector2f m_target;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    EnemyData m_data;

    float m_speed;
    float m_health;
    bool m_alive;
};
