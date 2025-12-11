#include "Engine/Pickup.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Resource.hpp"
#include "Engine/Utility.hpp"
#include "Engine/category.hpp"
#include "Game/DataTables.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <map>

std::map<Pickup_t, PickupData> PickupDataTable = initializePickupData();

Pickup::Pickup(Pickup_t type, const TextureHolder &textures)
    : Entity(1), mPickupType(type),
      mPickupSprite(textures.get(PickupDataTable[mPickupType].texture)) {
    centerOrigin(mPickupSprite);
}

GameObjectCategory Pickup::getCategory() const {
    return GameObjectCategory::Pickup;
}

sf::FloatRect Pickup::getBoundingBox() const {
    return getWorldTransform().transformRect(mPickupSprite.getGlobalBounds());
}

void Pickup::apply(Aircraft &player) const {
    PickupDataTable[mPickupType].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget &target,
                         sf::RenderStates states) const {
    target.draw(mPickupSprite, states);
}
