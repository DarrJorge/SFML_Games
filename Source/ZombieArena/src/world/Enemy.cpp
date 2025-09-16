//
// Created by Oleksandr Pogorelov.
//

#include "Enemy.h"

using namespace sf;

Enemy::Enemy(Sprite sprite, const Vector2f& position, const EnemyData& data)
    : m_sprite(std::move(sprite)), m_position(position), m_data(data), m_speed(data.speed), m_health(data.health)
{
    m_sprite.setOrigin(Vector2f{5.f, 5.f});
    m_alive = true;
}

FloatRect Enemy::getPosition() const
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

int32_t Enemy::attack() const
{
    return m_data.damage;
}