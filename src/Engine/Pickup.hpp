#pragma once

#include "Engine/Entity.hpp"
#include "Engine/Resource.hpp"
#include "Engine/category.hpp"
#include "Game/Aircraft.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

enum class Pickup_t { HealthBoost, MissilePickup, FireSpread, FireRate, count };

class Pickup : public Entity {
  public:
    Pickup(Pickup_t type, const TextureHolder &textures);

    virtual GameObjectCategory getCategory() const;
    virtual sf::FloatRect getBoundingBox() const;
    void apply(Aircraft &player) const;

  protected:
    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) const;

  private:
    Pickup_t mPickupType;
    sf::Sprite mPickupSprite;
};
