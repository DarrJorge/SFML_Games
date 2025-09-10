//
// Created by Oleksandr Pogorelov
//

#include "Bullet.h"


using namespace sf;

Bullet::Bullet()
{
    m_shape.setSize(Vector2f(4.f, 4.f));
}

void Bullet::shoot(const Vector2f& start, const Vector2f& target)
{
    m_inFlight = true;
    m_position.x = start.x;
    m_position.y = start.y;

    float gradient = (start.x - target.x) / (start.y - target.y);
    if (gradient < 0)
    {
        gradient *= -1;
    }

    float ratio = m_speed / (1 + gradient);
    m_distanceY = ratio;
    m_distanceX = ratio * gradient;

    if (target.x < start.x)
    {
        m_distanceX *= -1;
    }

    if (target.y < start.y)
    {
        m_distanceY *= -1;
    }

    float range = 1000;
    m_minX = start.x - range;
    m_maxX = start.x + range;
    m_minY = start.y - range;
    m_maxY = start.y + range;

    m_shape.setPosition(m_position);
}

void Bullet::stop()
{
    m_inFlight = false;
}

void Bullet::update(float deltaTime)
{
    if (!m_inFlight) return;

    m_position.x += m_distanceX * deltaTime;
    m_position.y += m_distanceY * deltaTime;

    m_shape.setPosition(m_position);

    if (m_position.x < m_minX || m_position.x > m_maxX || m_position.y < m_minY || m_position.y > m_maxY)
    {
        m_inFlight = false;
    }
}

FloatRect Bullet::getBounds() const
{
    return m_shape.getGlobalBounds();
}

Vector2f Bullet::getPosition() const
{
    return m_position;
}