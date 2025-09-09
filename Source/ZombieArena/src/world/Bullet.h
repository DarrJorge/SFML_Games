//
// Created by Oleksandr Pogorelov on 27.8.2025.
//

#pragma once

#include "SFML/Graphics.hpp"

class Bullet
{
public:
    Bullet();

    void shoot(const sf::Vector2f& start, const sf::Vector2f& target);
    void stop();
    void update(float deltaTime);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    bool isInFlight() const { return m_inFlight; }
    sf::RectangleShape getShape() const { return m_shape; }

private:
    sf::Vector2f m_position;
    sf::RectangleShape m_shape;

    bool m_inFlight{false};
    float m_speed{200.f};

    float m_distanceX;
    float m_distanceY;
    float m_maxX;
    float m_minX;
    float m_maxY;
    float m_minY;
};
