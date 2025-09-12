//
// Created by Oleksandr Pogorelov
//

#include "InputSystem.h"
#include "../core/Game.h"
#include "../utils/Utils.h"

using namespace sf;
using namespace ZombieArena::Utils;

InputSystem::InputSystem(EventBus& events)
    : m_events(events)
{
    m_startGameId = m_events.subscribe<ActionStartGame>(
            [this](const ActionStartGame& e){ m_queue.emplace(ActionStartGame{}); });

    m_pauseGameId = m_events.subscribe<ActionTogglePause>(
            [this](const ActionTogglePause& e){ m_queue.emplace(ActionTogglePause{}); });
}

InputSystem::~InputSystem()
{
    if (m_startGameId) m_events.unsubscribe<ActionStartGame>(m_startGameId);
    if (m_pauseGameId) m_events.unsubscribe<ActionTogglePause>(m_pauseGameId);
}

void InputSystem::handleEvent(const std::optional<sf::Event>& event)
{
    if (event == std::nullopt) return;

    if (auto* kp = event.value().getIf<Event::KeyPressed>())
    {
        if (kp->code == Keyboard::Key::Enter)
        {
            m_queue.emplace(ActionTogglePause{});
        }

        if (kp->code >= Keyboard::Key::Num1 && kp->code <= Keyboard::Key::Num6)
        {
            const int slot = static_cast<int>(kp->code) - static_cast<int>(Keyboard::Key::Num1);
            m_queue.emplace(ActionChooseUpgrade{ slot });
        }
    }

    if (auto* mouseButton = event.value().getIf<Event::MouseButtonPressed>())
    {
        if (mouseButton->button == Mouse::Button::Left)
        {
            m_queue.emplace(ActionStartFire{});
        }
    }

    if (auto* mouseButton = event.value().getIf<Event::MouseButtonReleased>())
    {
        if (mouseButton->button == Mouse::Button::Left)
        {
            m_queue.emplace(ActionStopFire{});
        }
    }
}

void InputSystem::pollRealtime(const RenderWindow& window)
{
    m_moveAxis = {0.f, 0.f};

    if (Keyboard::isKeyPressed(Keyboard::Key::A)) m_moveAxis.x -= 1.f;
    if (Keyboard::isKeyPressed(Keyboard::Key::D)) m_moveAxis.x += 1.f;
    if (Keyboard::isKeyPressed(Keyboard::Key::W)) m_moveAxis.y -= 1.f;
    if (Keyboard::isKeyPressed(Keyboard::Key::S)) m_moveAxis.y += 1.f;

    m_queue.emplace(ActionMoveAxis{m_moveAxis.x, m_moveAxis.y});
}

void InputSystem::dispatchActions(GameState state, World& world, Game& game)
{
    while (!m_queue.empty())
    {
        const Action a = std::move(m_queue.front());
        m_queue.pop();

        std::visit([&](auto&& act)
            {
                using T = std::decay_t<decltype(act)>;

                // --------------------
                // Pause / Resume toggle
                // --------------------
                if constexpr (std::is_same_v<T, ActionTogglePause>)
                {
                    if (state == GameState::START || state == GameState::GAME_OVER || state == GameState::WIN) return;
                    game.changeState(state == GameState::PLAYING ? GameState::PAUSED : GameState::PLAYING);
                }
                // --------------------
                // Start new run (from Start/GameOver screens)
                // --------------------
                else if constexpr (std::is_same_v<T, ActionStartGame>)
                {
                    if (state == GameState::START || state == GameState::GAME_OVER || state == GameState::WIN)
                        game.changeState(GameState::PLAYING);
                }
                // --------------------
                // Choose upgrade on LevelingUp
                // --------------------
                else if constexpr (std::is_same_v<T, ActionChooseUpgrade>)
                {
                    if (state == GameState::LEVELING_UP)
                    {
                        const int slot = act.slot;
                        game.changeState(GameState::PLAYING);
                    }
                }
                // --------------------
                // Fire control
                // --------------------
                else if constexpr (std::is_same_v<T, ActionStartFire>)
                {
                    if (state == GameState::PLAYING)
                    {
                        world.weapons().startFire(world.player());
                    }
                }
                else if constexpr (std::is_same_v<T, ActionStopFire>) {
                    // allow stop even if not Playing (safe)
                    world.weapons().stopFire(world.player());
                }
                // --------------------
                // Movement axis (WASD held)
                // --------------------
                else if constexpr (std::is_same_v<T, ActionMoveAxis>)
                {
                    // Normalize so diagonal speed == axial speed
                    sf::Vector2f axis = Utils::Normalize2D({act.x, act.y});
                    world.player().setMoveAxis({act.x, act.y});
                }
            }, a);
    }
}
