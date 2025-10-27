#pragma once

#include "Engine/Resource.hpp"
#include "Engine/Utility.hpp"
#include "Game/State.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

class TitleState : public State {
  public:
    TitleState(StateStack &stack, Context &context)
        : State(stack, context),
          mBackgroundSprite(context.textures->get(TextureId::TitleScreen)),
          mText(context.fonts->get(FontID::Main)), mShowText(true),
          mTextEffectTime(sf::Time::Zero) {

        mText.setString("Press any key to start");
        centerOrigin(mText);
        mText.setPosition(context.window->getView().getSize() / 2.f);
    }

    virtual void draw() {
        sf::RenderWindow &window = *getContext().window;
        window.draw(mBackgroundSprite);
        if (mShowText) {
            window.draw(mText);
        }
    };

    virtual bool update(sf::Time dt) {
        mTextEffectTime += dt;

        if (mTextEffectTime >= sf::seconds(0.5f)) {
            mShowText       = !mShowText;
            mTextEffectTime = sf::Time::Zero;
        }

        return true;
    }

    virtual bool handleEvent(const sf::Event &event) {
        // if any key is pressed continue to next screen
        if (event.is<sf::Event::KeyReleased>()) {
            requestStackPop();
            requestStackPush(StateID::Menu);
        }
        return true;
    }

  private:
    sf::Sprite mBackgroundSprite;
    sf::Text mText;
    bool mShowText;
    sf::Time mTextEffectTime;
};
