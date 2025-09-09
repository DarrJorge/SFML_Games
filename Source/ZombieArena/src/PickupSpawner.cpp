//
// Created by Oleksandr Pogorelov on 2.9.2025.
//

#include "PickupSpawner.h"
#include "ResourceManager.h"
#include "src/world/pickups/Pickup.h"

using namespace sf;


PickupSpawner::PickupSpawner(ResourceManager& resourceManager)
        //: m_rand(std::mt19937{ std::random_device{}() })
{
    data.insert({PickupType::HEALTH, {PickupType::HEALTH, 10, 5, 5}});
    data.insert({PickupType::AMMO, {PickupType::AMMO, 10, 5, 5}});

    m_texturesPickups.insert({PickupType::HEALTH, &resourceManager.getTexture("graphics/health_pickup.png")});
    m_texturesPickups.insert({PickupType::AMMO, &resourceManager.getTexture("graphics/ammo_pickup.png")});
}

void PickupSpawner::spawn(int count, const sf::IntRect& arenaRect)
{
    m_pickups.reserve(count);

    const size_t padding = 60;
    const size_t left = arenaRect.position.x + padding;
    const size_t right = arenaRect.position.x + arenaRect.size.x - padding;
    const size_t top = arenaRect.position.y + padding;
    const size_t bottom = arenaRect.position.y + arenaRect.size.y - padding;

    std::uniform_int_distribution<size_t> posXDist(left, right);
    std::uniform_int_distribution<size_t> posYDist(top, bottom);
    std::uniform_int_distribution<int> typeDist(0, static_cast<int>(PickupType::COUNT) - 1);

    for (int i = 0; i < count; ++i)
    {
        const float x = static_cast<float>(posXDist(m_rand));
        const float y = static_cast<float>(posYDist(m_rand));

        std::cout << "x:" << x << " y:" << y << std::endl;

        const auto type = static_cast<PickupType>(typeDist(m_rand));
        const PickupData& value = data.at(type);

        /*if (auto* pickup = new Pickup(*m_texturesPickups.at(type), value))
        {
            pickup->spawn(Vector2f(x, y));
            m_pickups.push_back(pickup);
        }*/
    }
}

void PickupSpawner::draw(RenderWindow& window)
{
    for (auto pickup : m_pickups)
    {
        if (!pickup->isSpawned()) continue;
        window.draw(pickup->getSprite());
    }
}
