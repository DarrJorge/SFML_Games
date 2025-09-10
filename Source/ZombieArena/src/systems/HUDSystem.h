//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../core/GameTypes.h"

class Game;
class World;
class EventBus;
struct GameStateChanged;

using namespace ZombieArena::Core::Types;


class HUDSystem
{
public:
    HUDSystem(World& world, EventBus& events, sf::Vector2u resolution);
    ~HUDSystem();

    void draw(sf::RenderWindow& window, GameState state);

private:
    void rebuildStaticLayout();
    void onGameStateChanged(GameState state);

    void updateHealthBar();
    void updateScoresForKills();
    void updateAmmoText();

private:
    World& m_world;
    EventBus& m_events;

    // subscribe to events
    std::size_t m_stateChangedSubId{0};
    std::size_t m_playerGotDamageId{0};
    std::size_t m_enemyKilledId{0};
    std::size_t m_shootId{0};
    std::size_t m_reloadWeaponId{0};

    // View HUD (screen)
    sf::View m_hudView;
    sf::Vector2u m_resolution{};

    sf::Font m_font;
    sf::Text m_pausedText;
    sf::Text m_levelUpText;
    sf::Text m_gameOverText;
    sf::Text m_ammoText;
    sf::Text m_enemiesRemainingText;

    sf::RectangleShape m_healthBarBg;
    sf::RectangleShape m_healthBar;

};
