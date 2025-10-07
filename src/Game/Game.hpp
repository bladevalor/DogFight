#pragma once

#include "Engine/CommandQueue.hpp"
#include "Engine/Player.hpp"
#include "Engine/World.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>

class Game {
  public:
    Game()
        : mWindow(sf::VideoMode({640, 480}), "World", sf::Style::Close),
          mWorld(mWindow), mFont(), mStatisticsText(mFont),
          mStatisticsUpdateTime(), mStatisticsNumFrames(0) {
        mWindow.setKeyRepeatEnabled(false);

        if (!mFont.openFromFile("assets/Sansation.ttf")) {
            throw std::runtime_error("ERROR: Cant open FONT file");
        };
        mStatisticsText.setFont(mFont);
        mStatisticsText.setPosition({5.f, 5.f});
        mStatisticsText.setCharacterSize(10);
    };

    Game(const Game &)            = delete; // disable copy constructor
    Game &operator=(const Game &) = delete; // disable copy operator

  public:
    void run() {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        while (mWindow.isOpen()) {
            sf::Time elapsedTime = clock.restart();
            timeSinceLastUpdate += elapsedTime;
            while (timeSinceLastUpdate > TimePerFrame) {
                timeSinceLastUpdate -= TimePerFrame;

                processInputs(mWindow);
                update(TimePerFrame);
            }

            updateStatistics(elapsedTime);
            render();
        }
    };

  private:
    void processInputs(sf::RenderWindow &window) {
        CommandQueue &commands = mWorld.getCommandQueue();

        const auto onClose     = [&](const sf::Event::Closed &event) {
            window.close();
        };

        const auto InputCommands =
            [&](const sf::Event::KeyPressed &keyPressed) {
                if (keyPressed.scancode == sf::Keyboard::Scancode::Escape ||
                    keyPressed.scancode == sf::Keyboard::Scancode::Q)
                    window.close();

                mPlayer.handleEvent(keyPressed.code, commands);
            };

        window.handleEvents(onClose, InputCommands);

        mPlayer.handleLiveInput(commands);
    };

    void update(sf::Time elapsedTime) { mWorld.update(elapsedTime); };

    void render() {
        mWindow.clear();
        mWorld.draw();

        mWindow.setView(mWindow.getDefaultView());
        mWindow.draw(mStatisticsText);
        mWindow.display();
    };

    void updateStatistics(sf::Time elapsedTime) {
        mStatisticsUpdateTime += elapsedTime;
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

  private:
    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow;
    World mWorld;

    sf::Font mFont;
    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;
    Player mPlayer;
};
