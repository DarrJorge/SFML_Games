//
// Created by Oleksandr Pogorelov.
//

#include "PickupFactory.h"
#include "../../core/SpawnConfig.h"
#include "ResourceManager.h"
#include "../../world/pickups/HealthPickup.h"
#include "../../world/pickups/AmmoPickup.h"

using namespace sf;
using namespace ZombieArena::Core;

namespace
{
    std::string_view pickupTexturePath(PickupType type)
    {
        switch (type)
        {
            case PickupType::AMMO: return Paths::Pickups::AMMO_PICKUP;
            case PickupType::HEALTH: return Paths::Pickups::HEALTH_PICKUP;
            default: return Paths::Pickups::AMMO_PICKUP;
        }
    }
}

PickupFactory::PickupFactory(const SpawnConfig& cfg)
 : m_config(cfg) {}

std::unique_ptr<Pickup> PickupFactory::make(PickupType type, const Vector2f& pos) const
{
    const PickupData& data = m_config.pickupData(type);
    auto& texture = ResourceManager::getTexture(pickupTexturePath(type));

    switch (type)
    {
        case PickupType::HEALTH:
            return std::make_unique<HealthPickup>(texture, pos, data.Value);
        case PickupType::AMMO:
            return std::make_unique<AmmoPickup>(texture, pos, data.Value);
        default: return nullptr;
    }
}
