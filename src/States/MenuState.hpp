#pragma once

#include "Engine/Resource.hpp"
#include "Game/State.hpp"
#include "Game/StateIDs.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstddef>
#include <vector>

class MenuState : public State {
  public:
    MenuState(StateStack &stack, Context context)
        : State(stack, context),
          mBackgroundSprite(context.textures->get(TextureId::TitleScreen)),
          mMenuOptions(), mOptionIndex(0) {

        sf::Text playOption(context.fonts->get(FontID::Main));
        playOption.setString("Play");
        playOption.setPosition(context.window->getView().getSize() / 2.f);
        mMenuOptions.push_back(playOption);

        sf::Text exitOption(context.fonts->get(FontID::Main));
        exitOption.setString("Exit");
        exitOption.setPosition(context.window->getView().getSize() / 2.f);
        mMenuOptions.push_back(exitOption);

        updateOptionText();
    }

    virtual void draw() override {
        sf::RenderWindow &window = *getContext().window;
        window.setView(window.getDefaultView());

        window.draw(mBackgroundSprite);

        for (auto &textOption : mMenuOptions) {
            window.draw(textOption);
        }
    }

    virtual bool update(sf::Time dt) override { return true; }

    virtual bool handleEvent(const sf::Event &event) override {
        const auto *key = event.getIf<sf::Event::KeyPressed>();
        if (key) {
            if (key->code == sf::Keyboard::Key::Enter) {
                if (mOptionIndex == (size_t)MenuState::MenuOptions::Play) {
                    requestStackPop();
                    requestStackPush(StateID::Game);
                } else if (mOptionIndex ==
                           (size_t)MenuState::MenuOptions::Exit) {
                    requestStackPop();
                }
            } else if (key->code == sf::Keyboard::Key::Up) {
                if (mOptionIndex > 0) {
                    mOptionIndex--;
                } else {
                    mOptionIndex = mMenuOptions.size() - 1;
                }

                updateOptionText();
            } else if (key->code == sf::Keyboard::Key::Down) {
                if (mOptionIndex < mMenuOptions.size() - 1) {
                    mOptionIndex++;
                } else {
                    mOptionIndex = 0;
                }

                updateOptionText();
            }
        }
        return true;
    }

    void updateOptionText() {
        if (mMenuOptions.empty()) {
            return;
        }
        for (auto &text : mMenuOptions) {
            text.setFillColor(sf::Color::White);
        }

        // Highlight selected "option" as red
        mMenuOptions[mOptionIndex].setFillColor(sf::Color::Red);
    }

  private:
    enum class MenuOptions { Play, Exit };

  private:
    sf::Sprite mBackgroundSprite;
    std::vector<sf::Text> mMenuOptions;
    std::size_t mOptionIndex;
};
