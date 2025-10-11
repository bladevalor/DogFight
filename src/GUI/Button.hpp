#pragma once

#include "Engine/Resource.hpp"
#include "Engine/Utility.hpp"
#include "GUI/Component.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

namespace GUI {
class Button : public Component {
  public:
    typedef std::shared_ptr<Button> ButtonPointer;
    typedef std::function<void()> Callback;

  public:
    Button(const FontHolder &fonts, const TextureHolder &textures)
        : mCallback(), mNormalTexture(textures.get(TextureId::ButtonNormal)),
          mSelectedTexture(textures.get(TextureId::ButtonSelected)),
          mPressedTexture(textures.get(TextureId::ButtonPressed)),
          mIsToggle(false), mSprite(textures.get(TextureId::ButtonNormal)),
          mText(fonts.get(FontID::Main), "", 16) {
        mSprite.setTexture(mNormalTexture);

        sf::Vector2f textBounds = mSprite.getLocalBounds().size;
        mText.setPosition(textBounds / 2.f);
    }

    void setCallback(Callback callback) { mCallback = std::move(callback); }

    void setText(const std::string &text) {
        mText.setString(text);
        centerOrigin(mText);
    }

    void setToggle(bool flag) { mIsToggle = flag; }

    virtual bool isSelectable() const { return true; }

    virtual void select() {
        Component::select();
        mSprite.setTexture(mSelectedTexture);
    }

    virtual void deselect() {
        Component::deselect();
        mSprite.setTexture(mNormalTexture);
    }

    virtual void activate() {
        Component::activate();

        if (mIsToggle) {
            mSprite.setTexture(mPressedTexture);
        }

        if (mCallback) {
            mCallback();
        }

        if (!mIsToggle) {
            deactivate();
        }
    }

    virtual void deactivate() {
        Component::deactivate();

        if (mIsToggle) {
            if (isSelected()) {
                mSprite.setTexture(mSelectedTexture);
            } else {
                mSprite.setTexture(mNormalTexture);
            }
        }
    }

    virtual void handleEvent(const sf::Event &event) {}

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(mSprite, states);
        target.draw(mText, states);
    }

  private:
    Callback mCallback;
    const sf::Texture &mNormalTexture;
    const sf::Texture &mSelectedTexture;
    const sf::Texture &mPressedTexture;
    sf::Sprite mSprite;
    sf::Text mText;
    bool mIsToggle;
};

} // namespace GUI
