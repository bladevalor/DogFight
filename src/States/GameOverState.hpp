#pragma once

#include "Engine/Player.hpp"
#include "Engine/Resource.hpp"
#include "Engine/Utility.hpp"
#include "Game/State.hpp"
#include "Game/StateIDs.hpp"
#include "Game/StateStack.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class GameOverState : public State {
  public:
    GameOverState(StateStack &stack, Context context)
        : State(stack, context),
          mGameOverText(context.fonts->get(FontID::Main), "", 70),
          mElapsedTime(sf::Time::Zero) {
        sf::Vector2f windowSize = context.window->getView().getSize();

        if (context.player->getMissionStatus() ==
            MissionStatus::MissionFailure) {
            mGameOverText.setString("Mission Failed!");
        } else {
            mGameOverText.setString("Mission Successful!");
        }
        centerOrigin(mGameOverText);
        mGameOverText.setPosition(0.5f * windowSize);
    }

    virtual void draw() {
        sf::RenderWindow &window = *getContext().window;

        // Dark semi-transparent backgound
        sf::RectangleShape backgroundShape;
        backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
        backgroundShape.setSize(window.getView().getSize());

        window.draw(backgroundShape);
        window.draw(mGameOverText);
    }

    virtual bool update(sf::Time dt) {
        mElapsedTime += dt;
        if (mElapsedTime > sf::seconds(3)) {
            requestStackClear();
            requestStackPush(StateID::Menu);
        }
        return false;
    }

    virtual bool handleEvent(const sf::Event &event) { return false; }

  private:
    sf::Text mGameOverText;
    sf::Time mElapsedTime;
};
