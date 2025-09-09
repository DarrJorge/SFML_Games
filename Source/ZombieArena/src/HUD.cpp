//
// Created by Oleksandr Pogorelov on 4.9.2025.
//

#include "HUD.h"
#include "ResourceManager.h"
#include <iostream>

using namespace sf;
using namespace ZombieArena;

HUD::HUD(const sf::Vector2u resolution, ResourceManager& resourceManager)
    : m_hudView(FloatRect(Vector2f(0, 0), Vector2f(resolution.x, resolution.y)))
    , m_gameOverSprite(resourceManager.getTexture(Core::Paths::BACKGROUND_UI_PATH))
    , m_ammoIcon(resourceManager.getTexture(Core::Paths::AMMO_ICON_PATH))
    , m_font(resourceManager.getFont(Core::Paths::FONT_PATH))
    , m_pausedText(m_font, "Press Enter to continue", 75)
    , m_gameOverText(m_font, "Press Enter to play", 125)
    , m_ammoText(m_font, "", 55)
    , m_scoreText(m_font, "", 55)
    , m_hitScoreText(m_font, "", 55)
    , m_enemiesRemainingText(m_font, "", 55)
    , m_waveNumberText(m_font, "", 55)
{
    const float offsetY = static_cast<float>(resolution.y - 60);
    const float resolutionXF = static_cast<float>(resolution.x);
    const float resolutionYF = static_cast<float>(resolution.y);

    m_gameOverSprite.setPosition({0.f, 0.f});
    m_ammoIcon.setPosition({20.f, offsetY});

    m_pausedText.setFillColor(Color::White);
    m_pausedText.setPosition(Vector2f{resolutionXF/2.f - m_pausedText.getGlobalBounds().size.x/2.f, resolutionYF/2.f});

    m_gameOverText.setFillColor(Color::White);
    m_gameOverText.setPosition(Vector2f{resolutionXF/2.f - m_gameOverText.getGlobalBounds().size.x/2.f, 850.f});

    //Text levelUpText(font, "Press Enter to play", 125);
    //levelUpText.setFillColor(Color::White);
    //levelUpText.setPosition(Vector2f{250.f, 850.f});

    m_ammoText.setFillColor(Color::White);
    m_ammoText.setPosition(Vector2f{200.f, offsetY});
    m_ammoText.setString("10/10");

    m_scoreText.setFillColor(Color::White);
    m_scoreText.setPosition(Vector2f{20.f, 0.f});
    m_scoreText.setString("Score: 0");

    m_hitScoreText.setFillColor(Color::White);
    m_hitScoreText.setPosition(Vector2f{resolutionXF - 300.f, 0.f});
    m_hitScoreText.setString("HitScore: 0");

    m_enemiesRemainingText.setFillColor(Color::White);
    m_enemiesRemainingText.setPosition(Vector2f{resolutionXF - 300.f, offsetY});
    m_enemiesRemainingText.setString("Enemies: 0");

    m_waveNumberText.setFillColor(Color::White);
    m_waveNumberText.setPosition(Vector2f{resolutionXF - 600.f, offsetY});
    m_waveNumberText.setString("WAVE: 0");

    m_healthBar.setFillColor(Color::Red);
    m_healthBar.setPosition({450.f, offsetY});
    m_healthBar.setSize(Vector2f{200.f, 50.f});
}

void HUD::setModel(const GameState& gameState)
{
    m_gameState = gameState;
}

void HUD::draw(RenderWindow& window)
{
    window.setView(m_hudView);

    if (m_gameState == GameState::PLAYING)
    {
        window.draw(m_scoreText);
        window.draw(m_ammoIcon);
        window.draw(m_hitScoreText);
        window.draw(m_ammoText);
        window.draw(m_enemiesRemainingText);
        window.draw(m_waveNumberText);
        window.draw(m_healthBar);
    }
    else if (m_gameState == GameState::LEVELING_UP)
    {
        window.draw(m_gameOverSprite);
    }
    else if (m_gameState == GameState::PAUSED)
    {
        window.draw(m_pausedText);
    }
    else if (m_gameState == GameState::GAME_OVER)
    {
        window.draw(m_gameOverSprite);
        window.draw(m_gameOverText);
        window.draw(m_scoreText);
        window.draw(m_hitScoreText);
    }
}