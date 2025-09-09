//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#pragma once

#include <queue>
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "../core/GameTypes.h"
#include "../core/Events.h"

class World;
class Game;

using namespace ZombieArena::Core::Types;

class InputSystem
{
public:
    explicit InputSystem(EventBus& events);

    void handleEvent(const std::optional<sf::Event>& event);
    void pollRealtime(const sf::RenderWindow& window);
    void dispatchActions(GameState state, World& world, Game& game);

private:
    EventBus& m_events;
    std::queue<Action> m_queue;
    sf::Vector2f m_moveAxis{};

};
