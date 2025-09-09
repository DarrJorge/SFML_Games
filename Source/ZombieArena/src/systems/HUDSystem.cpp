//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#include "HUDSystem.h"
#include <algorithm>
#include <cstdio>
#include "ResourceManager.h"
#include "../core/Events.h"
#include "../world/World.h"

using namespace sf;
using namespace ZombieArena::Core;

namespace {
    void MakeText(sf::Text& t, unsigned size, sf::Color color, const sf::Vector2f& pos, const std::string& str) {
        t.setCharacterSize(size);
        t.setFillColor(color);
        t.setPosition(pos);
        t.setString(str);
    }
}

/*
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
{}
 */

HUDSystem::HUDSystem(World& world, EventBus& events, sf::Vector2u res)
        : m_world(world)
        , m_events(events)
        , m_resolution(res)
        , m_hudView(FloatRect(Vector2f(0, 0), Vector2f(static_cast<float>(res.x), static_cast<float>(res.y))))
        , m_font(ResourceManager::getFont(Paths::FONT_PATH))
        , m_pausedText(m_font, "Press Enter to continue", 75)
        , m_gameOverText(m_font, "Press Enter to play", 125)
        , m_ammoText(m_font, "", 55)
        , m_levelUpText(m_font, "", 55)
        //, m_hitScoreText(m_font, "", 55)
        , m_enemiesRemainingText(m_font, "", 55)
        //, m_waveNumberText(m_font, "", 55)
{
    rebuildStaticLayout();
    refreshTexts();

    // Subscribe on state changes
    m_stateChangedSubId = m_events.subscribe<GameStateChangedEvent>(
            [this](const GameStateChangedEvent& e){ onGameStateChanged(e.newState); });
}

HUDSystem::~HUDSystem()
{
    if (m_stateChangedSubId)
    {
        m_events.unsubscribe<GameStateChangedEvent>(m_stateChangedSubId);
    }
}

void HUDSystem::rebuildStaticLayout()
{
    const float W = static_cast<float>(m_resolution.x);
    const float H = static_cast<float>(m_resolution.y);

    MakeText(m_pausedText, 96, Color::White, {W*0.5f - 300.f, H*0.4f}, "PAUSED\nPress Enter to continue");
    MakeText(m_levelUpText, 48, Color::White, {W*0.5f - 380.f, H*0.3f},
                              "LEVELING UP\nPress 1..6 to choose upgrade");
    MakeText(m_gameOverText, 96, Color::White, {W*0.5f - 350.f, H*0.4f}, "GAME OVER\nPress Enter to play");

    //m_healthText  = MakeText(m_font, 24, Color::White, {20.f, 16.f},  "HP: 100/100");
    MakeText(m_ammoText, 24, Color::White, {20.f, 48.f},  "Ammo: 6 (reloading: no)");
    MakeText(m_enemiesRemainingText, 24, Color::White, {20.f, 80.f},  "Zombies: 0");

    const Vector2f barPos{20.f, H - 40.f};
    const Vector2f barSizeBg{300.f, 20.f};

    m_healthBarBg.setPosition(barPos);
    m_healthBarBg.setSize(barSizeBg);
    m_healthBarBg.setFillColor(sf::Color(40, 40, 40));
    m_healthBarBg.setOutlineThickness(2.f);
    m_healthBarBg.setOutlineColor(sf::Color(80, 80, 80));

    m_healthBar.setPosition(barPos);
    m_healthBar.setSize({barSizeBg.x, barSizeBg.y});
    m_healthBar.setFillColor(sf::Color(200, 40, 40));
}

void HUDSystem::refreshTexts()
{
    // HEALTH
    const int hp  = m_world.player().getHealth();
    const int hpM = m_world.player().getMaxHealth();

    const float ratio = std::clamp(hpM > 0 ? (hp / static_cast<float>(hpM)) : 0.f, 0.f, 1.f);
    auto barSize = m_healthBarBg.getSize();
    m_healthBar.setSize({barSize.x * ratio, barSize.y});
    //char hpBuf[64];
    //std::snprintf(hpBuf, sizeof(hpBuf), "HP: %d/%d", hp, hpM);
    //m_healthText.setString(hpBuf);

    // AMMO
    const auto& weapons = m_world.weapons();
    const int inClip = weapons.inClip();
    const int clipSize = weapons.clipSize();
    //const bool reloading = weapons.reloading();

    char ammoBuf[96];
    std::snprintf(ammoBuf, sizeof(ammoBuf), "Ammo: %d/%d", inClip, clipSize);
    m_ammoText.setString(ammoBuf);

    int alive = 0;
    for (auto& e : m_world.enemies()) {
        if (e && e->isAlive()) ++alive;
    }
    char zBuf[64];
    std::snprintf(zBuf, sizeof(zBuf), "Zombies: %d", alive);
    m_enemiesRemainingText.setString(zBuf);
}

void HUDSystem::update(float deltaTime)
{
    m_updateAccum += deltaTime;
    if (m_updateAccum >= m_updateEvery)
    {
        refreshTexts();
        m_updateAccum = 0.f;
    }
}

void HUDSystem::draw(RenderWindow& window, GameState state)
{
    const sf::View prev = window.getView();
    window.setView(m_hudView);

    window.draw(m_healthBarBg);
    window.draw(m_healthBar);
    //window.draw(m_healthText);
    window.draw(m_ammoText);
    window.draw(m_enemiesRemainingText);

    switch (state)
    {
        case GameState::PAUSED:
            window.draw(m_pausedText);
            break;
        case GameState::LEVELING_UP:
            window.draw(m_levelUpText);
            break;
        case GameState::GAME_OVER:
            window.draw(m_gameOverText);
            break;
        default:
            break;
    }
    window.setView(prev);
}

void HUDSystem::onGameStateChanged(GameState state)
{
    refreshTexts();
}