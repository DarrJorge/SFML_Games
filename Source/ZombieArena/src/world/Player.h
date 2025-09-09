#pragma once

#include "SFML/Graphics.hpp"
#include <memory>
#include "WeaponSystem.h"

class Pickup;

struct Stats
{
    std::uint32_t maxHealth{10};
    float speed{200.f};
};


/// Player
/// -------
/// - Holds movement axis (from input), world position, aim position
/// - Updates position each fixed tick
/// - Keeps health/invincibility timer
/// - Exposes AABB bounds and sprite for rendering
class Player
{

public:
    explicit Player(const Stats& stats);

    void spawn(const sf::Vector2f& spawnPosition);
    void setArena(const sf::IntRect& arena);
    void update(float deltaTime);
    void resetTimers();

    void setMoveAxis(const sf::Vector2f& axis);
    void setAimPosition(const sf::Vector2f& world);

    bool hit(int damage);
    bool heal(int hp);
    uint32_t getHealth() const { return m_health; }
    uint32_t getMaxHealth() const { return m_stats.maxHealth; }
    bool isAlive() const { return m_health > 0; }

    bool tryApplyPickup(Pickup* pickup);

    sf::Vector2f getCenter() const { return m_position; }
    sf::Vector2f getAimPosition() const { return m_aimWorld; }

    sf::FloatRect getBounds() const;
    const sf::Sprite& getSprite() const { return m_sprite; }

    void setRotateToAim(bool enabled) { m_rotateToAim = enabled; }

    void setSpeed(float s) { m_speed = s; }
    float speed() const { return m_speed; }
    bool isDied() const { return m_health == 0; }

private:
    void updateVisuals();
    void clampInsideArena();

private:
    Stats m_stats{};

    sf::Vector2f m_position{0.f, 0.f};  // world center of the player
    sf::Vector2f m_moveAxis{0.f, 0.f};  // normalized input axis
    sf::Vector2f m_aimWorld{0.f, 0.f};  // world-space aim (mouse)

    // Invincibility after hit (seconds)
    float m_invincibleFor{0.0f};
    const float INVINCIBLE_DURATION = 0.6f;

    // Visuals
    sf::Texture m_texture;
    sf::Sprite  m_sprite;
    bool m_rotateToAim{true};

    sf::IntRect m_arena{};
    bool m_hasArena{false};

    uint32_t m_health{0};
    float m_speed{0};
};