#include "Player.h"
#include "ResourceManager.h"
#include "src/core/GameTypes.h"
#include "src/world/pickups/Pickup.h"
#include "../utils/Utils.h"
#include <cmath>
#include <algorithm>

using namespace sf;
using namespace ZombieArena::Utils;
using namespace ZombieArena::Core;

Player::Player(const Stats& stats)
    : m_stats(stats), m_sprite(ResourceManager::getTexture(Paths::PLAYER_AVATAR_PATH))
{
    m_sprite.setOrigin({25.f, 25.f});

    m_health = m_stats.maxHealth;
    m_speed = m_stats.speed;

    const auto bounds = m_sprite.getLocalBounds();
    m_position = {bounds.size.x * 0.5f, bounds.size.y * 0.5f};
    m_sprite.setOrigin(m_position);

    // Initial positions
    m_aimWorld = m_position;
}

void Player::spawn(const Vector2f& spawnPosition)
{
    m_position = spawnPosition;
    resetTimers();
    updateVisuals();
}

void Player::setArena(const IntRect& arena)
{
    m_arena = arena;
    m_hasArena = true;
}

void Player::resetTimers()
{
    m_invincibleFor = 0.f;
}

void Player::setMoveAxis(const Vector2f& axis)
{
    m_moveAxis = Utils::Normalize2D(axis);
}

void Player::setAimPosition(const Vector2f& worldPos)
{
    m_aimWorld = worldPos;
}

bool Player::hit(int damage)
{
    if (damage <= 0) return false;
    if (m_invincibleFor > 0.f) return false; // ignore during i-frames

    const auto hit = static_cast<int>(m_health - damage);
    m_health = std::max(0, hit);
    m_invincibleFor = INVINCIBLE_DURATION;
    std::cout << "health: " << m_health << std::endl;
    return m_health == 0;
}

bool Player::heal(int hp)
{
    if (hp <= 0 || m_health == m_stats.maxHealth) return false;
    m_health = std::min(m_stats.maxHealth, m_health + hp);
    return true;
}

bool Player::tryApplyPickup(Pickup* pickup)
{
    if (!pickup) return false;
    return pickup->applyTo(*this);
}

void Player::update(float deltaTime)
{
    if (isDied()) return;

    // Timers
    if (m_invincibleFor > 0.f)
    {
        m_invincibleFor -= deltaTime;
        if (m_invincibleFor < 0.f)
            m_invincibleFor = 0.f;
    }

    const Vector2f velocity = { m_moveAxis.x * m_speed, m_moveAxis.y * m_speed };
    m_position += velocity * deltaTime;

    if (m_hasArena)
    {
        clampInsideArena();
    }

    updateVisuals();
}

FloatRect Player::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

void Player::updateVisuals()
{
    m_sprite.setPosition(m_position);

    if (m_rotateToAim)
    {
        const Vector2f direction = { m_aimWorld.x - m_position.x, m_aimWorld.y - m_position.y };
        const float rad = atan2(direction.y, direction.x);
        m_sprite.setRotation(sf::radians(rad));
    }
}

void Player::clampInsideArena()
{
    if (!m_hasArena) return;

    const auto local = m_sprite.getLocalBounds();
    const Vector2f half{ local.size.x * 0.3f, local.size.y * 0.3f };

    float minX = static_cast<float>(m_arena.position.x) + half.x + Constants::TILE_SIZE;
    float maxX = static_cast<float>(m_arena.position.x + m_arena.size.x) - half.x - Constants::TILE_SIZE;
    float minY = static_cast<float>(m_arena.position.y) + half.y + Constants::TILE_SIZE;
    float maxY = static_cast<float>(m_arena.position.y + m_arena.size.y) - half.y - Constants::TILE_SIZE;

    m_position.x = std::clamp(m_position.x, minX, maxX);
    m_position.y = std::clamp(m_position.y, minY, maxY);
}

