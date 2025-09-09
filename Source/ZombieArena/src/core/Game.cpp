//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#include "Game.h"
#include "ResourceManager.h"
#include <iostream>

using namespace sf;

Game::Game(RenderWindow& window, Vector2u resolution)
    : m_window(window)
    , m_resolution(resolution)
    , m_events()
    , m_world(resolution, m_events)
    , m_input(m_events)
    , m_spawn(m_world, m_events, 5.f)
    , m_physics(m_world, m_events)
    , m_combat(m_world, m_events)
    , m_render(m_world)
    , m_hud(m_world, m_events, resolution)
{
}

Game::~Game()
{
    ResourceManager::clear();
}

void Game::init()
{
    const IntRect arenaSize({0, 0}, {800, 800});
    m_world.buildArena(arenaSize);

    // Center player at arena center
    const auto& arena = m_world.arena();
    const sf::Vector2f spawnPos{
            static_cast<float>(arena.position.x + arena.size.x  / 2),
            static_cast<float>(arena.position.y + arena.size.y / 2)
    };
    m_world.player().spawn(spawnPos);
    m_world.player().setArena(arena);

    changeState(GameState::LEVELING_UP);
}

void Game::fixedUpdate(float deltaTime)
{
    m_input.dispatchActions(m_gameState, m_world, *this);

    Vector2i mouseScreen;
    Vector2f mouseWorld;

    switch (m_gameState)
    {
        case GameState::PLAYING:
            m_world.update(deltaTime);
            m_spawn.update(deltaTime);
            m_physics.update(deltaTime);
            m_combat.update(deltaTime);

            mouseScreen = Mouse::getPosition(m_window);
            mouseWorld = m_window.mapPixelToCoords(mouseScreen, m_render.mainView());
            m_world.player().setAimPosition(mouseWorld);
            break;

        case GameState::LEVELING_UP:
            // TODO implementation will be later
            break;

        case GameState::GAME_OVER:
            // TODO implementation will be later
            break;
    }
    m_hud.update(deltaTime);
}

void Game::render(RenderWindow& window)
{
    m_render.drawWorld(window, m_gameState);
    m_hud.draw(window, m_gameState);
}

void Game::changeState(GameState nextState)
{
    m_gameState = nextState;
    m_events.emit(GameStateChangedEvent{m_gameState});
    if (m_gameState == GameState::PLAYING)
    {
        m_world.resetClocksGameplay();
    }
}