//
// Created by Oleksandr Pogorelov on 5.9.2025.
//

#pragma once

#include <variant>
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <cstddef>
#include "GameTypes.h"

// ---------------------
// Actions (game input)
// ---------------------
struct ActionTogglePause {};
struct ActionStartGame {};
struct ActionChooseUpgrade { int slot; };
struct ActionStartFire {};
struct ActionStopFire {};
struct ActionMoveAxis { float x, y; };

using Action = std::variant<ActionTogglePause, ActionStartGame, ActionChooseUpgrade, ActionStartFire,
                ActionStopFire, ActionMoveAxis>;


// ---------------------
// Game events (system)
// ---------------------
struct GameStateChangedEvent { ZombieArena::Core::Types::GameState newState; };
struct BulletHitEnemyEvent { size_t bulletIdx; size_t enemyIdx;};
struct PlayerTouchedPickupEvent { size_t pickupIdx;};
struct PickupCollectedEvent { size_t pickupIdx;};
struct EnemyHitPlayerEvent { size_t enemyIdx; };
struct ReloadWeaponEvent {};
struct ShootEvent {};
struct PlayerGetDamageEvent{};
struct PlayerDiedEvent{};
struct EnemyKilledEvent{};
struct AllEnemiesDied{};


class EventBus
{
public:
    EventBus() = default;
    ~EventBus() = default;

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    template<class Event>
    size_t subscribe(std::function<void(const Event&)> handler)
    {
        auto& vec = m_subscribers[typeid(Event)];
        size_t id = ++m_lastId;
        vec.push_back({id, [handler](const void* ev) {
            handler(*static_cast<const Event*>(ev));
        }});
    }

    template<class Event>
    void unsubscribe(size_t id)
    {
        auto it = m_subscribers.find(typeid(Event));
        if (it == m_subscribers.end()) return;
        auto& vec = it->second;
        vec.erase(std::remove_if(vec.begin(), vec.end(), [id](const Subscriber& s){
            return s.id == id;
        }), vec.end());
    }

    template<class Event>
    void emit(const Event& e)
    {
        auto it = m_subscribers.find(typeid(Event));
        if (it == m_subscribers.end()) return;
        for (auto& sub : it->second)
        {
            sub.fn(&e);
        }
    }

private:
    struct Subscriber
    {
        size_t id;
        std::function<void(const void*)> fn;
    };

    std::unordered_map<std::type_index, std::vector<Subscriber>> m_subscribers;
    size_t m_lastId{0};
};
