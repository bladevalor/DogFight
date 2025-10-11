#pragma once

#include "Engine/Player.hpp"
#include "Engine/Resource.hpp"
#include "GUI/Button.hpp"
#include "GUI/Container.hpp"
#include "GUI/Label.hpp"
#include "Game/State.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <map>
#include <memory>

class SettingsState : public State {
  public:
    SettingsState(StateStack &stack, Context context)
        : State(stack, context), mGUIContainer(),
          mBackgroundSprite(context.textures->get(TextureId::TitleScreen)) {
        addButtonLabel(InputAction::moveLeft, 150.f, "Move Left", context);
        addButtonLabel(InputAction::moveRight, 200.f, "Move Right", context);
        addButtonLabel(InputAction::moveUp, 250.f, "Move Up", context);
        addButtonLabel(InputAction::moveDown, 300.f, "Move Down", context);

        updateLabels();

        auto backButton =
            std::make_shared<GUI::Button>(*context.fonts, *context.textures);
        backButton->setPosition({80.f, 375.f});
        backButton->setText("Back");
        backButton->setCallback(
            std::bind(&SettingsState::requestStackPop, this));

        mGUIContainer.pack(backButton);
    }

  public:
    virtual void draw() override {
        sf::RenderWindow &window = *getContext().window;

        window.draw(mBackgroundSprite);
        window.draw(mGUIContainer);
    }

    virtual bool update(sf::Time dt) override { return true; }

    virtual bool handleEvent(const sf::Event &event) override {
        bool isKeyBinding = false;

        for (auto action : allInputActions) {
            if (mBindingButtons[action]->isActive()) {
                isKeyBinding = true;
                if (auto *key = event.getIf<sf::Event::KeyReleased>()) {
                    getContext().player->assignKey(action, key->code);
                    mBindingButtons[action]->deactivate();
                }
                break;
            }
        }

        if (isKeyBinding) {
            updateLabels();
        } else {
            mGUIContainer.handleEvent(event);
        }

        return false;
    }

  private:
    void updateLabels() {
        Player &player = *getContext().player;

        for (auto action : allInputActions) {
            sf::Keyboard::Key key = player.getAssignedKey(action);
            std::string stringKey =
                sf::Keyboard::getDescription(sf::Keyboard::delocalize(key));
            mBindingLabels[action]->setText(stringKey);
        }
    }

    void addButtonLabel(InputAction action, float y, const std::string &text,
                        Context context) {
        mBindingButtons[action] =
            std::make_shared<GUI::Button>(*context.fonts, *context.textures);

        mBindingButtons[action]->setPosition({80.f, y});
        mBindingButtons[action]->setText(text);
        mBindingButtons[action]->setToggle(true);

        mBindingLabels[action] =
            std::make_shared<GUI::Label>("", *context.fonts);
        mBindingLabels[action]->setPosition({300.f, y + 15.f});

        mGUIContainer.pack(mBindingButtons[action]);
        mGUIContainer.pack(mBindingLabels[action]);
    }

  private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
    std::map<InputAction, GUI::Button::ButtonPointer> mBindingButtons;
    std::map<InputAction, GUI::Label::LabelPointer> mBindingLabels;
};
