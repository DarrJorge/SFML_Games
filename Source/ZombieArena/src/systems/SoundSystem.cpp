//
// Created by Oleksandr Pogorelov.
//

#include "SoundSystem.h"
#include "../core/Events.h"
#include "ResourceManager.h"

using namespace sf;
using namespace ZombieArena::Core::Types;

namespace
{
    Sound& acquireVoice(std::vector<Sound>& voices, const SoundBuffer& buffer)
    {
        if (auto it = std::ranges::find_if(voices,
            [](Sound& s) { return s.getStatus() == Sound::Status::Stopped; }); it != voices.end())
        {
            return *it;
        }

        Sound sound(buffer);
        voices.emplace_back(sound);
        return voices.back();
    }
}

SoundSystem::SoundSystem(EventBus& events)
    : m_events(events)
{
    m_buffers.insert({SoundType::ENEMY_HIT, &ResourceManager::getSound("sound/splat.wav")});
    m_buffers.insert({SoundType::PLAYER_HIT, &ResourceManager::getSound("sound/hit.wav")});
    m_buffers.insert({SoundType::PICKUP, &ResourceManager::getSound("sound/pickup.wav")});
    m_buffers.insert({SoundType::SHOOT, &ResourceManager::getSound("sound/shoot.wav")});
    m_buffers.insert({SoundType::RELOAD_WEAPON, &ResourceManager::getSound("sound/reload.wav")});
    m_buffers.insert({SoundType::POWERUP, &ResourceManager::getSound("sound/powerup.wav")});
    m_buffers.insert({SoundType::NO_AMMO, &ResourceManager::getSound("sound/reload_failed.wav")});

    m_voices.reserve(static_cast<int>(SoundType::COUNT) - 1);

    subscribe();
}

SoundSystem::~SoundSystem()
{
    unsubscribe();
}

void SoundSystem::subscribe()
{
    m_subEnemyHitId = m_events.subscribe<BulletHitEnemyEvent>([this](const BulletHitEnemyEvent&){
        onPlaySound(*m_buffers.at(SoundType::ENEMY_HIT));
    });

    m_subPickupId = m_events.subscribe<PickupCollectedEvent>([this](const PickupCollectedEvent&){
        onPlaySound(*m_buffers.at(SoundType::PICKUP));
    });

    m_reloadWeaponId = m_events.subscribe<ReloadWeaponEvent>([this](const ReloadWeaponEvent&){
        onPlaySound(*m_buffers.at(SoundType::RELOAD_WEAPON));
    });

    m_shootId = m_events.subscribe<ShootEvent>([this](const ShootEvent&){
        onPlaySound(*m_buffers.at(SoundType::SHOOT));
    });

    m_playerGetDamageId = m_events.subscribe<PlayerGetDamageEvent>([this](const PlayerGetDamageEvent&){
        onPlaySound(*m_buffers.at(SoundType::PLAYER_HIT));
    });

}

void SoundSystem::unsubscribe()
{
    if (m_subEnemyHitId) m_events.unsubscribe<BulletHitEnemyEvent>(m_subEnemyHitId);
    if (m_subPickupId) m_events.unsubscribe<PickupCollectedEvent>(m_subPickupId);
    if (m_reloadWeaponId) m_events.unsubscribe<ReloadWeaponEvent>(m_reloadWeaponId);
    if (m_shootId) m_events.unsubscribe<ShootEvent>(m_shootId);
    if (m_playerGetDamageId) m_events.unsubscribe<PlayerGetDamageEvent>(m_playerGetDamageId);
}

void SoundSystem::onPlaySound(const SoundBuffer& buffer)
{
    Sound& voice = acquireVoice(m_voices, buffer);
    voice.setBuffer(buffer);
    voice.setVolume(m_volume);
    voice.play();
}