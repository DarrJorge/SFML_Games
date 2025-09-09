//
// Created by Oleksandr Pogorelov on 24.8.2025.
//

#include "Enemy.h"
#include <numbers>

using namespace sf;

Enemy::Enemy(const Sprite& sprite, const Vector2f& position, float speed, float health)
    : m_sprite(sprite), m_position(position), m_speed(speed), m_health(health)
{
    m_sprite.setOrigin(Vector2f{5.f, 5.f});
    m_alive = true;
}

sf::FloatRect Enemy::getPosition() const
{

    return m_sprite.getGlobalBounds();
}

bool Enemy::hit(float amount)
{
    m_health -= amount;
    if (m_health <= 0.0f)
    {
        m_alive = false;
        return true;
    }
    return false;
}

void Enemy::update(float elapsedTime)
{
    Vector2f to = m_target - m_position;
    const float distance = std::hypot(to.x, to.y);

    if (distance > 0.f)
    {
        const Vector2f direction{to.x/distance, to.y/distance};
        const float step = m_speed * elapsedTime;

        if (step >= distance) m_position = m_target;
        else m_position = {m_position.x + direction.x * step, m_position.y + direction.y * step};

        m_sprite.setRotation(sf::radians(atan2(direction.y, direction.x)));
    }

    m_sprite.setPosition(m_position);
}

void Enemy::setTarget(const sf::Vector2f& target)
{
    m_target = target;
}

int Enemy::attack() const
{
    return 2; // @TODO need different damage from different types enemy
}