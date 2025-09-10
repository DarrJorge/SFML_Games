//
// Created by Oleksandr Pogorelov on 5.9.2025.
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

private:
    sf::RenderWindow& m_window;
    sf::Vector2u m_resolution;
    GameState m_gameState{GameState::START};

    EventBus m_events;
    World m_world;

    // systems
    InputSystem m_input;
    SpawnSystem m_spawn;
    PhysicsSystem m_physics;
    CombatSystem m_combat;
    RenderSystem m_render;
    HUDSystem m_hud;
    SoundSystem m_sound;
};
