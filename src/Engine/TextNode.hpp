#pragma once

#include "Engine/Resource.hpp"
#include "Engine/SceneNode.hpp"
#include "Engine/Utility.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

class TextNode : public SceneNode {
  public:
    explicit TextNode(const FontHolder &fonts, const std::string &text)
        : mText(fonts.get(FontID::Main), text, 20) {}

    void setString(const std::string &text) {
        mText.setString(text);
        centerOrigin(mText);
    }

  private:
    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) {
        target.draw(mText, states);
    }

  private:
    sf::Text mText;
};
