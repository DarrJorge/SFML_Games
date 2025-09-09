//
// Created by Oleksandr Pogorelov on 4.9.2025.
//

#pragma once

#include <SFML/Graphics.hpp>
#include "core/GameTypes.h"

class ResourceManager;

using namespace sf;
using namespace ZombieArena::Core::Types;

class HUD
{
public:
    HUD(const sf::Vector2u resolution, ResourceManager& resourceManager);
    void setModel(const GameState& gameState);

    void draw(sf::RenderWindow& window);

private:
    GameState m_gameState;

    View m_hudView;
    Sprite m_gameOverSprite;
    Sprite m_ammoIcon;

    Font m_font;

    Text m_pausedText;
    Text m_gameOverText;
    Text m_ammoText;
    Text m_scoreText;
    Text m_hitScoreText;
    Text m_enemiesRemainingText;
    Text m_waveNumberText;

    RectangleShape m_healthBar;

};