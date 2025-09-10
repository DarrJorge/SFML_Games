//
// Created by Oleksandr Pogorelov
//

#pragma once

#include <SFML/Graphics.hpp>
#include "../world/World.h"
#include "../core/GameTypes.h"

using namespace ZombieArena::Core::Types;

class RenderSystem
{
public:
    explicit RenderSystem(World& world);
    void drawWorld(sf::RenderWindow& window, GameState gameState);

    const sf::View& mainView() const { return m_gameView; }

private:
    World& m_world;
    sf::View m_gameView;
};
