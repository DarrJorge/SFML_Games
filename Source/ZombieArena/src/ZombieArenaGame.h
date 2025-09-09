//
// Created by Oleksandr Pogorelov on 24.8.2025.
//

#pragma once

#include <SFML/Graphics.hpp>
#include "core/GameTypes.h"

using namespace ZombieArena::Core::Types;
using namespace sf;

class ZombieArenaGame
{
public:
    void run();

private:

    GameState m_gameState{GameState::START};
    Clock m_clock;

};