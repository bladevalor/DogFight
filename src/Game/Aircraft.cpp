#include "Game/Aircraft.hpp"

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

Aircraft::Aircraft(Aircraft_t type, const TextureHolder &textures)
    : mType(type), mSprite(textures.get(toTextureId(type))) {
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.size / 2.f);
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
