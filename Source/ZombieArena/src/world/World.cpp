//
// Created by Oleksandr Pogorelov.
//

#include "World.h"
#include "ResourceManager.h"
#include "../core/Game.h"

using namespace sf;
using namespace ZombieArena::Core;

World::World(Game& game, Vector2u resolution, EventBus& events)
    : m_game(game), m_resolution(resolution), m_events(events)
{
    m_player = std::make_unique<Player>(Stats{10, 200});
    m_weapons = std::make_unique<WeaponSystem>(events);

    m_playerDiedId = m_events.subscribe<PlayerDiedEvent>([this](const PlayerDiedEvent&){
        m_game.changeState(GameState::GAME_OVER);
        reset();
    });

    m_gameStateId = m_events.subscribe<GameStateChangedEvent>([this](const GameStateChangedEvent& e){
        if (e.newState == Types::GameState::WIN)
        {
            reset();
        }
    });
}

World::~World()
{
    if (m_playerDiedId) m_events.unsubscribe<PlayerDiedEvent>(m_playerDiedId);
    if (m_gameStateId) m_events.unsubscribe<GameStateChangedEvent>(m_gameStateId);
}

void World::init(const sf::IntRect& rectArena)
{
    buildArena(rectArena);
    spawnPlayer();
}

void World::spawnPlayer()
{
    // Center player at arena center
    const sf::Vector2f spawnPos{
            static_cast<float>(m_arena.position.x + m_arena.size.x  / 2),
            static_cast<float>(m_arena.position.y + m_arena.size.y / 2)
    };
    player().reset();
    player().spawn(spawnPos);
    player().setArena(m_arena);
}

void World::buildArena(const IntRect& rect)
{
    using namespace Constants;

    m_arena = rect;

    const auto worldWidth = m_arena.size.x / TILE_SIZE;
    const auto worldHeight = m_arena.size.y / TILE_SIZE;
    m_background.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_background.resize(worldWidth * worldHeight * VERTS_IN_TILE);

    std::uniform_int_distribution<int> floorDist(0, TILE_TYPES - 1); // 3 tile variants
    size_t v = 0;

    for (size_t x = 0; x < worldWidth; ++x)
    {
        for (size_t y = 0; y < worldHeight; ++y)
        {
            const auto xPos = static_cast<float>(x * TILE_SIZE);
            const auto yPos = static_cast<float>(y * TILE_SIZE);

            Vector2f topLeft(xPos, yPos);
            Vector2f topRight(xPos + TILE_SIZE, yPos);
            Vector2f bottomLeft(xPos, yPos + TILE_SIZE);
            Vector2f bottomRight(xPos + TILE_SIZE, yPos + TILE_SIZE);

            // Triangle 1
            m_background[v + 0].position = topLeft;
            m_background[v + 1].position = topRight;
            m_background[v + 2].position = bottomLeft;
            // Triangle 2
            m_background[v + 3].position = bottomLeft;
            m_background[v + 4].position = topRight;
            m_background[v + 5].position = bottomRight;

            const bool isWall =
                    (y == 0) || (y == worldHeight - 1) ||
                    (x == 0) || (x == worldWidth - 1);

            const int vOffset = isWall ? (TILE_TYPES * TILE_SIZE) : floorDist(m_rng) * TILE_SIZE;

            const float v0 = static_cast<float>(vOffset);
            const float v1 = static_cast<float>(vOffset + TILE_SIZE);
            const float u0 = 0.f;
            const float u1 = static_cast<float>(TILE_SIZE);

            m_background[v + 0].texCoords = {u0, v0};   // topLeft
            m_background[v + 1].texCoords = {u1, v0};   // topRight
            m_background[v + 2].texCoords = {u0, v1};   // bottomLeft
            m_background[v + 3].texCoords = {u0, v1};   // bottomLeft
            m_background[v + 4].texCoords = {u1, v0};   // topRight
            m_background[v + 5].texCoords = {u1, v1};   // bottomRight

            v += VERTS_IN_TILE;
        }
    }

    m_bgTexture = Texture(ResourceManager::getTexture(Paths::BACKGROUND_GAME_PATH));
}

void World::update(float deltaTime)
{
    if (m_player)
    {
        m_player->update(deltaTime);
    }

    for (auto& enemy : m_enemies)
    {
        if (enemy->isAlive())
        {
            enemy->setTarget(m_player->getCenter());
            enemy->update(deltaTime);
        }
    }

    if (m_weapons)
    {
        m_weapons->update(deltaTime);
    }
}

void World::resetClocksGameplay()
{
    if (m_player)
    {
        m_player->resetTimers();
    }
}

void World::addScore(size_t scores)
{
    m_scores += scores;
}

bool World::detectCollisionWalls(const sf::Vector2f& position) const
{
    float minX = static_cast<float>(m_arena.position.x) + Constants::TILE_SIZE;
    float maxX = static_cast<float>(m_arena.position.x + m_arena.size.x) - Constants::TILE_SIZE;
    float minY = static_cast<float>(m_arena.position.y) + Constants::TILE_SIZE;
    float maxY = static_cast<float>(m_arena.position.y + m_arena.size.y) - Constants::TILE_SIZE;

    return (position.y <= minY) || (position.y >= maxY) || (position.x <= minX) || (position.x >= maxX);
}

void World::reset()
{
    enemies().clear();
    pickups().clear();
    weapons().reset();
}