#pragma once

#include "Engine/ParallelTask.hpp"
#include "Engine/Resource.hpp"
#include "Game/State.hpp"
#include "Game/StateIDs.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class LoadingState : public State {
  public:
    LoadingState(StateStack &stack, Context context)
        : State(stack, context),
          mLoadingText(context.fonts->get(FontID::Main)) {
        sf::RenderWindow &window    = *getContext().window;
        sf::Vector2f viewDimensions = window.getView().getSize();

        mLoadingText.setString("Loading Resources");
        mLoadingText.setPosition(
            {viewDimensions.x / 2.f, viewDimensions.y / 2.f + 50.f});

        mProgressBarBackground.setFillColor(sf::Color::White);
        mProgressBarBackground.setSize({viewDimensions.x - 20, 10});
        mProgressBarBackground.setPosition(
            {10, mLoadingText.getPosition().y + 40});

        mProgressBar.setFillColor(sf::Color(100, 100, 100));
        mProgressBar.setSize({200, 10});
        mProgressBar.setPosition({10, mLoadingText.getPosition().y + 40});

        setCompletion(0.f);

        mLoadingTask.execute();
    }

    virtual void draw() {
        sf::RenderWindow &window = *getContext().window;
        window.setView(window.getDefaultView());

        window.draw(mLoadingText);
        window.draw(mProgressBarBackground);
        window.draw(mProgressBar);
    }

    virtual bool update(sf::Time dt) {
        if (!mLoadingTask.isFinished()) {
            // keep updating the progressBar
            setCompletion(mLoadingTask.getCompletion());
        } else {
            requestStackPop();
            requestStackPush(StateID::Game);
        }
        return true;
    }

    virtual bool handleEvent(const sf::Event &event) { return true; }

    void setCompletion(float percent) {
        // clamp to 1(100%)
        if (percent > 1) {
            percent = 1;
        }
        mProgressBar.setSize({mProgressBarBackground.getSize().x * percent,
                              mProgressBar.getSize().y});
    }

  private:
    sf::Text mLoadingText;
    sf::RectangleShape mProgressBarBackground;
    sf::RectangleShape mProgressBar;

    ParallelTask mLoadingTask;
};
