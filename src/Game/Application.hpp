#pragma once

#include "Engine/Player.hpp"
#include "Engine/Resource.hpp"
#include "Game/State.hpp"
#include "Game/StateIDs.hpp"
#include "Game/StateStack.hpp"
#include "States/GameState.hpp"
#include "States/MenuState.hpp"
#include "States/PauseState.hpp"
#include "States/TitleState.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstdio>
#include <optional>
#include <string>

sf::Font static mStatFont("assets/Sansation.ttf");
sf::Text static mStatisticsText(mStatFont);

class Application {
  public:
    Application()
        : mWindow(sf::VideoMode({640, 480}), "World", sf::Style::Close),
          mFonts(), mTextures(),
          // chad
          // mStatisticsText(mFonts.get(FontID::Main)),
          // mStatisticsText(sf::Font*("chad")),
          // chad
          mPlayer(),
          mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer)),
          mStatisticsUpdateTime(), mStatisticsNumFrames(0) {
        mWindow.setKeyRepeatEnabled(false);

        mFonts.load(FontID::Main, "assets/Sansation.ttf");
        mTextures.load(TextureId::TitleScreen, "assets/TitleScreen.png");

        // mStatisticsText.emplace(mFonts.get(FontID::Main));
        mStatisticsText.setPosition({5.f, 5.f});
        mStatisticsText.setCharacterSize(10);

        registerStates();
        mStateStack.pushState(StateID::Title);
    };

    Application(const Application &) = delete; // disable copy constructor
    Application &
    operator=(const Application &) = delete; // disable copy operator

  public:
    void run() {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        while (mWindow.isOpen()) {
            sf::Time dt = clock.restart();
            timeSinceLastUpdate += dt;
            while (timeSinceLastUpdate > TimePerFrame) {
                timeSinceLastUpdate -= TimePerFrame;

                processInputs();
                update(TimePerFrame);

                // close Game if there are no States left
                if (mStateStack.isEmpty()) {
                    mWindow.close();
                }
            }

            updateStatistics(dt);
            render();
        }
    };

  private:
    void processInputs() {
        while (const std::optional<sf::Event> event = mWindow.pollEvent()) {
            mStateStack.handleEvent(*event);

            if (event->is<sf::Event::Closed>()) {
                mWindow.close();
            } else if (const auto *key =
                           event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Q) {
                    mWindow.close();
                }
            }
        }
    };

    void update(sf::Time dt) { mStateStack.update(dt); };

    void render() {
        mWindow.clear();
        mStateStack.draw();

        mWindow.setView(mWindow.getDefaultView());
        mWindow.draw(mStatisticsText);
        mWindow.display();
    };

    void updateStatistics(sf::Time dt) {
        mStatisticsUpdateTime += dt;
        mStatisticsNumFrames += 1;

        if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
            mStatisticsText.setString(
                "FPS = " + std::to_string(mStatisticsNumFrames) + "\n" +
                "render_delta = " +
                std::to_string(mStatisticsUpdateTime.asMicroseconds() /
                               mStatisticsNumFrames) +
                "us");

            mStatisticsUpdateTime -= sf::seconds(1.0f);
            mStatisticsNumFrames = 0;
        }
    };

    void registerStates() {
        mStateStack.registerState<TitleState>(StateID::Title);
        mStateStack.registerState<MenuState>(StateID::Menu);
        mStateStack.registerState<GameState>(StateID::Game);
        mStateStack.registerState<PauseState>(StateID::Pause);
    }

  private:
    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow;
    FontHolder mFonts;
    TextureHolder mTextures;
    Player mPlayer;

    StateStack mStateStack;

    // std::optional<sf::Text> mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;
};
