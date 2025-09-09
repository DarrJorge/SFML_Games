//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#pragma once

#include <random>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../core/Events.h"
#include "Player.h"
#include "src/world/pickups/Pickup.h"
#include "Enemy.h"
#include "WeaponSystem.h"


class World
{
public:
    World(sf::Vector2u resolution, EventBus& events);

    void buildArena(const sf::IntRect& rect);
    void update(float deltaTime);
    void resetClocksGameplay();
    void addScore(size_t scores);
    bool detectCollisionWalls(const sf::Vector2f& position) const;

    std::vector<std::unique_ptr<Enemy>>& enemies() { return m_enemies; }
    std::vector<std::unique_ptr<Pickup>>& pickups() { return m_pickups; }

    Player& player() { return *m_player; }
    const Player& player() const { return *m_player; }
    WeaponSystem& weapons() { return *m_weapons; }
    const WeaponSystem& weapons() const { return *m_weapons; }
    const sf::IntRect& arena() const { return m_arena; }
    size_t getScores() const { return m_scores; }

    std::mt19937& rng() { return m_rng; }

private:
    EventBus& m_events;
    sf::Vector2u m_resolution;
    sf::IntRect m_arena{};

    std::mt19937 m_rng{std::random_device{}()};
    std::unique_ptr<Player> m_player;
    std::unique_ptr<WeaponSystem> m_weapons;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::unique_ptr<Pickup>> m_pickups;

    sf::VertexArray m_background;
    sf::Texture m_bgTexture;

    size_t m_scores{0};

    friend class RenderSystem;
};