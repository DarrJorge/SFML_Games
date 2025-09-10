//
// Created by Oleksandr Pogorelov.
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