//
// Created by Oleksandr Pogorelov
//

#include "Game.h"
#include "ResourceManager.h"

using namespace sf;
using namespace ZombieArena::Core::Constants;

Game::Game(RenderWindow& window, Vector2u resolution)
    : m_window(window)
    , m_resolution(resolution)
    , m_events()
    , m_world(*this, resolution, m_events)
    , m_menu(m_resolution, m_events)
    , m_input(m_events)
    , m_enemyFactory(m_spawnConfig)
    , m_pickupFactory(m_spawnConfig)
    , m_spawn(m_world, m_events, m_spawnConfig, m_enemyFactory, m_pickupFactory, 5.f)
    , m_physics(m_world, m_events)
    , m_combat(m_world, m_events)
    , m_render(m_world)
    , m_hud(m_world, m_events, resolution)
    , m_sound(m_events)
{
    m_WaveFinishedId = m_events.subscribe<AllEnemiesDied>([this](const AllEnemiesDied&){
        changeState(GameState::WIN);
    });
}

Game::~Game()
{
    ResourceManager::clear();

    if (m_WaveFinishedId) m_events.unsubscribe<AllEnemiesDied>(m_WaveFinishedId);
}

void Game::init()
{
    const IntRect initialArenaSize({0, 0}, {800, 800});
    m_world.init(initialArenaSize);
    m_spawn.spawnNextWave(m_currentWaveIndex, BASE_ENEMIES_COUNT, BASE_PICKUPS_COUNT);
}

void Game::fixedUpdate(float deltaTime)
{
    m_input.dispatchActions(m_gameState, m_world, *this);

    Vector2i mouseScreen;
    Vector2f mouseWorld;

    m_menu.update(m_window);

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
    }
}

void Game::render(RenderWindow& window)
{
    if (m_gameState == GameState::PLAYING || m_gameState == GameState::PAUSED)
    {
        m_render.drawWorld(window, m_gameState);
        m_hud.draw(window, m_gameState);
    }
    m_menu.draw(window);
}

void Game::changeState(GameState nextState)
{
    switch (nextState)
    {
        case GameState::PLAYING:
            if (m_gameState == GameState::WIN || m_gameState == GameState::GAME_OVER)
            {
                m_world.spawnPlayer();
                m_spawn.spawnNextWave(m_currentWaveIndex, BASE_ENEMIES_COUNT, BASE_PICKUPS_COUNT);
            }

            m_menu.setVisible(false);
            m_world.resetClocksGameplay();
            break;

        case GameState::PAUSED:
            m_menu.setVisible(true);
            m_menu.setContinueEnabled(true);
            break;

        case GameState::LEVELING_UP:
            // TODO implementation will be later
            break;

        case GameState::GAME_OVER:
            m_menu.setVisible(true);
            m_menu.setLoseScreen();
            break;

        case GameState::WIN:
            m_menu.setVisible(true);
            m_menu.setWinScreen();
            break;
    }
    m_gameState = nextState;
    m_events.emit(GameStateChangedEvent{m_gameState});
}