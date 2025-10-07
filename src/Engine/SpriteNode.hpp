#pragma once

#include "SceneNode.hpp"
#include <SFML/Graphics.hpp>

class SpriteNode : public SceneNode {
  public:
    SpriteNode(const sf::Texture &texture) : mSprite(texture) {};
    SpriteNode(const sf::Texture &texture, const sf::IntRect &rect)
        : mSprite(texture, rect) {};

  private:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(mSprite, states);
    }

  private:
    sf::Sprite mSprite;
};
