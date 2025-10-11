#pragma once

#include "Engine/Resource.hpp"
#include "GUI/Component.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

namespace GUI {

class Label : public Component {
  public:
    typedef std::shared_ptr<Label> LabelPointer;

  public:
    Label(const std::string &text, const FontHolder &fonts)
        : mText(fonts.get(FontID::Main), text, 16) {}

    virtual bool isSelectable() const { return false; }

    void setText(const std::string &text) {
        mText.setString(text);
        // centerOrigin(mText);
    }

    virtual void handleEvent(const sf::Event &event) {}

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(mText, states);
    }

  private:
    sf::Text mText;
};

} // namespace GUI
