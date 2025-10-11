#pragma once

#include "Engine/Resource.hpp"
#include "GUI/Button.hpp"
#include "GUI/Container.hpp"
#include "Game/State.hpp"
#include "Game/StateIDs.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

class MenuState : public State {
  public:
    MenuState(StateStack &stack, Context context)
        : State(stack, context),
          mBackgroundSprite(context.textures->get(TextureId::TitleScreen)) {

        auto playButton =
            std::make_shared<GUI::Button>(*context.fonts, *context.textures);
        playButton->setPosition({100, 250});
        playButton->setText("Play");
        playButton->setCallback([this]() {
            requestStackPop();
            requestStackPush(StateID::Game);
        });

        auto settingsButton =
            std::make_shared<GUI::Button>(*context.fonts, *context.textures);
        settingsButton->setPosition({100, 300});
        settingsButton->setText("Settings");
        settingsButton->setCallback(
            [this]() { requestStackPush(StateID::Setting); });

        auto exitButton =
            std::make_shared<GUI::Button>(*context.fonts, *context.textures);
        exitButton->setPosition({100, 350});
        exitButton->setText("Exit");
        exitButton->setCallback([this]() { requestStackPop(); });

        mGUIContainer.pack(playButton);
        mGUIContainer.pack(settingsButton);
        mGUIContainer.pack(exitButton);
    }

    virtual void draw() override {
        sf::RenderWindow &window = *getContext().window;
        window.setView(window.getDefaultView());

        window.draw(mBackgroundSprite);
        window.draw(mGUIContainer);
    }

    virtual bool update(sf::Time dt) override { return true; }

    virtual bool handleEvent(const sf::Event &event) override {
        mGUIContainer.handleEvent(event);
        return false;
    }

  private:
    enum class MenuOptions { Play, Exit };

  private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
};
