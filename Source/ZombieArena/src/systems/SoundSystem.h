//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include "../core/GameTypes.h"

using namespace ZombieArena::Core::Types;

class EventBus;

class SoundSystem
{
public:
    SoundSystem(EventBus& events);
    ~SoundSystem();

private:
    void subscribe();
    void unsubscribe();
    void onPlaySound(const sf::SoundBuffer& sound);

private:

    EventBus& m_events;
    std::vector<sf::Sound> m_voices;
    std::map<SoundType, const sf::SoundBuffer*> m_buffers;

    float m_volume{100.f};

    std::size_t m_subEnemyHitId{0};
    std::size_t m_subPickupId{0};
    std::size_t m_reloadWeaponId{0};
    std::size_t m_shootId{0};
    std::size_t m_playerGetDamageId{0};
};
