//
// Created by Oleksandr Pogorelov
//

#include "ZombieArenaGame.h"
#include "core/Game.h"
#include "systems/InputSystem.h"

using namespace ZombieArena::Core;
using namespace ZombieArena::Core::Types;
using namespace sf;

void ZombieArenaGame::run()
{
    VideoMode videoMode = VideoMode::getDesktopMode();
    RenderWindow window(videoMode, "Zombie Arena", Style::Default);
    window.setVerticalSyncEnabled(true);

    Game game(window, {videoMode.size});
    game.init();

    const float deltaTime = 1.f / 60.f;
    float acc = 0.f;
    Clock clock;

    while (window.isOpen())
    {
        // Input
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
            }
            game.input().handleEvent(event);
        }
        game.input().pollRealtime(window);

        // Update
        acc += clock.restart().asSeconds();
        while (acc >= deltaTime)
        {
            game.fixedUpdate(deltaTime);
            acc -= deltaTime;
        }

        // Render
        window.clear();
        game.render(window);
        window.display();
    }
}