#pragma once

#include "Engine/Utility.hpp"
#include "Game/State.hpp"
#include "Game/StateIDs.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

class PauseState : public State {
  public:
    PauseState(StateStack &stack, Context context)
        : State(stack, context),
          mBackgroundSprite(context.textures->get(TextureId::TitleScreen)),
          mPausedText(context.fonts->get(FontID::Main)),
          mInstructionText(context.fonts->get(FontID::Main)) {
        sf::Vector2f viewDimension = context.window->getView().getSize();

        mPausedText.setString("Game Paused");
        mPausedText.setCharacterSize(70);
        centerOrigin(mPausedText);
        mPausedText.setPosition(
            {05.f * viewDimension.x, 04.f * viewDimension.y});

        mInstructionText.setString(
            "(Press Backspace to return to the main menu)");
        centerOrigin(mInstructionText);
        mInstructionText.setPosition(
            {0.5f * viewDimension.x, 0.6f * viewDimension.y});
    }

    virtual void draw() {
        sf::RenderWindow &window = *getContext().window;
        window.setView(window.getDefaultView());

        sf::RectangleShape backgroundShape;
        backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
        backgroundShape.setSize((sf::Vector2f)window.getSize());

        window.draw(backgroundShape);
        window.draw(mPausedText);
        window.draw(mInstructionText);
    }

    virtual bool update(sf::Time dt) { return false; }

    virtual bool handleEvent(const sf::Event &event) {
        if (!event.is<sf::Event::KeyPressed>()) {
            return false;
        }

        const auto *key = event.getIf<sf::Event::KeyPressed>();
        if (key->code == sf::Keyboard::Key::Backspace) {
            requestStackClear();
            requestStackPush(StateID::Menu);
        } else if (key->code == sf::Keyboard::Key::Escape) {
            requestStackPop();
        }
        return true;
    }

  private:
    sf::Sprite mBackgroundSprite;
    sf::Text mPausedText;
    sf::Text mInstructionText;
};
