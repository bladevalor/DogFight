#pragma once

#include "Engine/Entity.hpp"
#include "Engine/Resource.hpp"
#include "Engine/TextNode.hpp"
#include "Engine/category.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

enum class Aircraft_t { Eagle, Raptor };
static std::vector<Aircraft_t> allAircraftTypes = {Aircraft_t::Eagle,
                                                   Aircraft_t::Raptor};

class Aircraft : public Entity {
  public:
    Aircraft(Aircraft_t type, const TextureHolder &textures,
             const FontHolder &fonts);

    void drawCurrent(sf::RenderTarget &target,
                     sf::RenderStates states) const override;

    void update();

    GameObjectCategory getCategory() const override;

  private:
    Aircraft_t mType;
    sf::Sprite mSprite;
    TextNode *mHealthDisplay;
};
