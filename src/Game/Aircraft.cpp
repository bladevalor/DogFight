#include "Game/Aircraft.hpp"
#include "Engine/Resource.hpp"
#include "Engine/Utility.hpp"
#include "Game/DataTables.hpp"
#include <map>
#include <memory>
#include <string>

TextureId toTextureId(Aircraft_t type) {
    switch (type) {
    case Aircraft_t::Eagle:
        return TextureId::Eagle;
    case Aircraft_t::Raptor:
        return TextureId::Raptor;
    default:
        return TextureId::None;
    }
}

std::map<Aircraft_t, AircraftData> aircraftDataTable = initializeAircraftData();

Aircraft::Aircraft(Aircraft_t type, const TextureHolder &textures,
                   const FontHolder &fonts)
    : Entity(aircraftDataTable[type].hitpoints), mType(type),
      mSprite(textures.get(toTextureId(type))) {
    centeOrigin(mSprite);

    std::unique_ptr<TextNode> healthDisplay =
        std::make_unique<TextNode>(fonts, "");
    mHealthDisplay = healthDisplay.get();
    addChild(std::move(healthDisplay));
};

void Aircraft::drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const {
    target.draw(mSprite, states);
}

GameObjectCategory Aircraft::getCategory() const {
    switch (mType) {
    case Aircraft_t::Eagle:
        return GameObjectCategory::PlayerAircraft;
    default:
        return GameObjectCategory::EnemyAircraft;
    }
}

void Aircraft::update() {
    mHealthDisplay->setString(std::to_string(getHintpoints()) + " HP");
    mHealthDisplay->setPosition({0.f, 50.f});
    mHealthDisplay->setRotation(-getRotation());
}
