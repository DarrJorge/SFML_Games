//
// Created by Oleksandr Pogorelov.
//

#include "HUDSystem.h"
#include <algorithm>
#include "ResourceManager.h"
#include "../core/Events.h"
#include "../world/World.h"

using namespace sf;
using namespace ZombieArena::Core;

namespace {
    void MakeText(sf::Text& t, unsigned size, sf::Color color, const sf::Vector2f& pos, const std::string& str) {
        t.setCharacterSize(size);
        t.setFillColor(color);
        t.setPosition(pos);
        t.setString(str);
    }
}

HUDSystem::HUDSystem(World& world, EventBus& events, sf::Vector2u res)
        : m_world(world)
        , m_events(events)
        , m_resolution(res)
        , m_hudView(FloatRect(Vector2f(0, 0), Vector2f(static_cast<float>(res.x), static_cast<float>(res.y))))
        , m_font(ResourceManager::getFont(Paths::FONT_PATH))
        , m_ammoText(m_font, "", 55)
        , m_enemiesRemainingText(m_font, "", 55)
{
    // Subscribe on state changes
    m_stateChangedSubId = m_events.subscribe<GameStateChangedEvent>(
            [this](const GameStateChangedEvent& e){ onGameStateChanged(e.newState); });

    m_playerGotDamageId = m_events.subscribe<PlayerGetDamageEvent>(
            [this](const PlayerGetDamageEvent& e){ updateHealthBar(); });

    m_enemyKilledId = m_events.subscribe<EnemyKilledEvent>(
            [this](const EnemyKilledEvent& e){ updateScoresForKills(); });

    m_shootId = m_events.subscribe<ShootEvent>(
            [this](const ShootEvent& e){ updateAmmoText(); });

    m_reloadWeaponId = m_events.subscribe<ReloadWeaponEvent>(
            [this](const ReloadWeaponEvent& e){ updateAmmoText(); });
}

HUDSystem::~HUDSystem()
{
    if (m_stateChangedSubId) m_events.unsubscribe<GameStateChangedEvent>(m_stateChangedSubId);
    if (m_playerGotDamageId) m_events.unsubscribe<PlayerGetDamageEvent>(m_playerGotDamageId);
    if (m_enemyKilledId) m_events.unsubscribe<EnemyKilledEvent>(m_enemyKilledId);
    if (m_shootId) m_events.unsubscribe<ShootEvent>(m_shootId);
    if (m_reloadWeaponId) m_events.unsubscribe<ReloadWeaponEvent>(m_reloadWeaponId);
}

void HUDSystem::rebuildStaticLayout()
{
    const float W = static_cast<float>(m_resolution.x);
    const float H = static_cast<float>(m_resolution.y);

    MakeText(m_ammoText, 24, Color::White, {20.f, 48.f},  "");
    MakeText(m_enemiesRemainingText, 24, Color::White, {20.f, 80.f},  "");

    const Vector2f barPos{20.f, H - 40.f};
    const Vector2f barSizeBg{300.f, 20.f};

    m_healthBarBg.setPosition(barPos);
    m_healthBarBg.setSize(barSizeBg);
    m_healthBarBg.setFillColor(sf::Color(40, 40, 40));
    m_healthBarBg.setOutlineThickness(2.f);
    m_healthBarBg.setOutlineColor(sf::Color(80, 80, 80));

    m_healthBar.setPosition(barPos);
    m_healthBar.setSize({barSizeBg.x, barSizeBg.y});
    m_healthBar.setFillColor(sf::Color(200, 40, 40));
}

void HUDSystem::draw(RenderWindow& window, GameState state)
{
    const sf::View prev = window.getView();
    window.setView(m_hudView);

    window.draw(m_healthBarBg);
    window.draw(m_healthBar);
    window.draw(m_ammoText);
    window.draw(m_enemiesRemainingText);

    window.setView(prev);
}

void HUDSystem::onGameStateChanged(GameState state)
{
    if (state == GameState::PLAYING)
    {
        rebuildStaticLayout();
        updateHealthBar();
        updateScoresForKills();
        updateAmmoText();
    }
}

void HUDSystem::updateHealthBar()
{
    auto& player = m_world.player();
    const float value = player.getMaxHealth() > 0 ? (player.getHealth() / static_cast<float>(player.getMaxHealth())) : 0.f;
    const float ratio = std::clamp(value, 0.f, 1.f);
    auto barSize = m_healthBarBg.getSize();
    m_healthBar.setSize({barSize.x * ratio, barSize.y});
}

void HUDSystem::updateScoresForKills()
{
    const int alive = std::count_if(m_world.enemies().begin(), m_world.enemies().end(),
        [](const auto& enemy){ return enemy && enemy->isAlive();});

    m_enemiesRemainingText.setString(std::format("Zombies: {}", alive));
}

void HUDSystem::updateAmmoText()
{
    const auto& weapons = m_world.weapons();
    m_ammoText.setString(std::format("Ammo: {}/{}", weapons.inClip(), weapons.clipSize()));
}