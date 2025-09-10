//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#include "Game.h"
#include "ResourceManager.h"

using namespace sf;

Game::Game(RenderWindow& window, Vector2u resolution)
    : m_window(window)
    , m_resolution(resolution)
    , m_events()
    , m_world(*this, resolution, m_events)
    , m_input(m_events)
    , m_spawn(m_world, m_events, 5.f)
    , m_physics(m_world, m_events)
    , m_combat(m_world, m_events)
    , m_render(m_world)
    , m_hud(m_world, m_events, resolution)
    , m_sound(m_events)
{
}

Game::~Game()
{
    ResourceManager::clear();
}

void Game::init()
{
    const IntRect initialArenaSize({0, 0}, {800, 800});
    m_world.init(initialArenaSize);

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