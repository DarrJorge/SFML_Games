//
// Created by Oleksandr Pogorelov.
//

#include "EnemyFactory.h"
#include "../../world/Enemy.h"
#include "../../core/SpawnConfig.h"
#include "ResourceManager.h"
#include <string_view>

using namespace sf;
using namespace ZombieArena::Core;

namespace
{
    std::string_view enemyTexturePath(EEnemyType type)
    {
        switch (type)
        {
            case EEnemyType::BLOATER: return Paths::Enemies::ENEMY_BLOATER;
            case EEnemyType::CRAWLER: return Paths::Enemies::ENEMY_CRAWLER;
            default: return Paths::Enemies::ENEMY_CHASER;
        }
    }
}

EnemyFactory::EnemyFactory(const SpawnConfig& config) : m_config(config)
{
}

std::unique_ptr<Enemy> EnemyFactory::make(EEnemyType type, const Vector2f& pos) const
{
    const EnemyData& data = m_config.enemyData(type);
    auto sprite = makeSprite(type);
    sprite.setPosition(pos);
    return std::make_unique<Enemy>(std::move(sprite), pos, data);
}

Sprite EnemyFactory::makeSprite(EEnemyType type) const
{
    auto& texture = ResourceManager::getTexture(enemyTexturePath(type));
    Sprite enemySprite(texture);

    const auto b = enemySprite.getLocalBounds();
    enemySprite.setOrigin({b.size.x * .5f, b.size.y * .5f});
    return enemySprite;
}

