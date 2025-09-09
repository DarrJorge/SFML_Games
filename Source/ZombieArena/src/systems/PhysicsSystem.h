//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#pragma once

#include "../world/World.h"
#include "../core/Events.h"

class PhysicsSystem
{
public:
    PhysicsSystem(World& world, EventBus& events);
    void update(float deltaTime);

private:
    World& m_world;
    EventBus& m_events;
};
