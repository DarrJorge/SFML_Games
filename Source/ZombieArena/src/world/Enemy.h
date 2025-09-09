//
// Created by Oleksandr Pogorelov on 24.8.2025.
//

#pragma once

#include "SFML/Graphics.hpp"

class Enemy
{
public:
    Enemy(const sf::Sprite& sprite, const sf::Vector2f& position, float speed, float health);

    bool hit(float amount);
    void update(float elapsedTime);
    void setTarget(const sf::Vector2f& target);
    int attack() const;

    sf::FloatRect getPosition() const;
    bool isAlive() const { return m_alive; }
    sf::Sprite getSprite() const { return m_sprite; }

private:

    sf::Vector2f m_target;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    float m_speed;
    float m_health;
    bool m_alive;
};
