//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#include "RenderSystem.h"

using namespace sf;

RenderSystem::RenderSystem(World& world)
    : m_world(world)
    , m_gameView()
{
}

void RenderSystem::drawWorld(RenderWindow& window, GameState gameState)
{
    m_gameView = View({m_world.player().getCenter()}, Vector2f{window.getSize()});
    window.setView(m_gameView);

    window.draw(m_world.m_background, &m_world.m_bgTexture);

    for (auto& enemy : m_world.enemies())
    {
        if (!enemy || !enemy->isAlive()) continue;
        window.draw(enemy->getSprite());
    }

    for (auto& bullet : m_world.weapons().bullets())
    {
        if (!bullet.isInFlight()) continue;
        window.draw(bullet.getShape());
    }

    for (auto& pickup : m_world.pickups())
    {
        if (!pickup || !pickup->isSpawned()) continue;
        window.draw(pickup->getSprite());
    }

    window.draw(m_world.player().getSprite());
}