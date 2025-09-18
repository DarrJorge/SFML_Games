//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <array>
#include <unordered_map>
#include "GameTypes.h"

using namespace ZombieArena::Core::Types;


class SpawnConfig
{
public:
    SpawnConfig();

    [[nodiscard]] const std::array<EEnemyType, 3>& enemyTypes() const { return m_enemyTypes; }
    [[nodiscard]] const std::array<PickupType, 2>& pickupTypes() const { return m_pickupTypes; }

    const EnemyData& enemyData(EEnemyType t) const;
    const PickupData& pickupData(PickupType t) const;

private:

    std::unordered_map<EEnemyType, EnemyData> m_enemyData;
    std::unordered_map<PickupType, PickupData> m_pickupData;

    std::array<EEnemyType, 3> m_enemyTypes { EEnemyType::BLOATER, EEnemyType::CHASER, EEnemyType::CRAWLER };
    std::array<PickupType, 2> m_pickupTypes { PickupType::HEALTH, PickupType::AMMO };
};