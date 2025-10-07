#pragma once

#include "Engine/Entity.hpp"
#include "Engine/Resource.hpp"
#include "Engine/category.hpp"
#include <SFML/Graphics.hpp>

enum class Aircraft_t { Eagle, Raptor };

class Aircraft : public Entity {
  public:
    Aircraft(Aircraft_t type, const TextureHolder &textures);

    void drawCurrent(sf::RenderTarget &target,
                     sf::RenderStates states) const override;

    GameObjectCategory getCategory() const override;

  private:
    Aircraft_t mType;
    sf::Sprite mSprite;
};
