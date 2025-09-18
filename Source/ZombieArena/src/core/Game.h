//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <SFML/Graphics.hpp>
#include "GameTypes.h"
#include "Events.h"
#include "../world/World.h"
#include "../systems/InputSystem.h"
#include "../systems/SpawnSystem.h"
#include "../systems/PhysicsSystem.h"
#include "../systems/CombatSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/HUDSystem.h"
#include "../systems/SoundSystem.h"
#include "../ui/MenuSystem.h"
#include "SpawnConfig.h"
#include "../systems/factory/EnemyFactory.h"
#include "../systems/factory/PickupFactory.h"

using namespace ZombieArena::Core::Types;

class Game
{
public:
    explicit Game(sf::RenderWindow& window, sf::Vector2u resolution);
    ~Game();

    void init();
    void fixedUpdate(float deltaTime);
    void render(sf::RenderWindow& window);
    void changeState(GameState nextState);

    InputSystem& input() { return m_input; }
    MenuSystem& menu() { return m_menu; }

private:
    sf::RenderWindow& m_window;
    sf::Vector2u m_resolution;
    GameState m_gameState{GameState::START};

    EventBus m_events;
    World m_world;
    MenuSystem m_menu;

    // systems
    InputSystem m_input;
    SpawnSystem m_spawn;
    PhysicsSystem m_physics;
    CombatSystem m_combat;
    RenderSystem m_render;
    HUDSystem m_hud;
    SoundSystem m_sound;

    EnemyFactory m_enemyFactory;
    PickupFactory m_pickupFactory;
    SpawnConfig m_spawnConfig;

    int m_currentWaveIndex{1};

    size_t m_WaveFinishedId{0};
};
