//
// Created by Oleksandr Pogorelov.
//

#include "SpawnConfig.h"
#include <stdexcept>

SpawnConfig::SpawnConfig()
{
    // {enemyType, { hp, speed, damage}}
    m_enemyData.insert({EEnemyType::BLOATER, {2, 5, 3}});
    m_enemyData.insert({EEnemyType::CHASER, {5, 1, 1}});
    m_enemyData.insert({EEnemyType::CRAWLER, {3, 3, 2}});

    // {pickupType, value, respawnSec, Duration}
    m_pickupData.insert({PickupType::HEALTH, {PickupType::HEALTH, 10, 5, 5}});
    m_pickupData.insert({PickupType::AMMO, {PickupType::AMMO, 10, 5, 5}});
}

const EnemyData& SpawnConfig::enemyData(EEnemyType t) const
{
    auto it = m_enemyData.find(t);
    if (it == m_enemyData.end())
        throw std::runtime_error("EnemyData not found");
    return it->second;
}

const PickupData& SpawnConfig::pickupData(PickupType t) const
{
    auto it = m_pickupData.find(t);
    if (it == m_pickupData.end())
        throw std::runtime_error("PickupData not found");
    return it->second;
}