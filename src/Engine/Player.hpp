#pragma once

#include "Engine/Command.hpp"
#include "Engine/CommandQueue.hpp"
#include "Game/Aircraft.hpp"
#include "category.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

enum class InputAction { moveLeft, moveRight, moveDown, moveUp };

struct AircraftMover {
    AircraftMover(float vx, float vy) : velocity({vx, vy}) {};

    void operator()(Aircraft &aircraft, sf::Time) const {
        aircraft.accelerate(velocity);
    };
    sf::Vector2f velocity;
};

class Player {
  public:
    Player() {
        mKeyBinding[sf::Keyboard::Key::Left]  = InputAction::moveLeft;
        mKeyBinding[sf::Keyboard::Key::Right] = InputAction::moveRight;
        mKeyBinding[sf::Keyboard::Key::Down]  = InputAction::moveDown;
        mKeyBinding[sf::Keyboard::Key::Up]    = InputAction::moveUp;

        initializeActions();

        for (auto &pair : mActionBinding) {
            pair.second.category = GameObjectCategory::PlayerAircraft;
        }
    };

    void handleEvent(sf::Keyboard::Key key, CommandQueue &commands) {
        auto found = mKeyBinding.find(key);
        if (found != mKeyBinding.end() && isLiveAction(found->second)) {
            commands.push(mActionBinding[found->second]);
        }
    }

    void handleLiveInput(CommandQueue &commands) {
        for (auto &pair : mKeyBinding) {
            if (sf::Keyboard::isKeyPressed(pair.first) &&
                isLiveAction(pair.second)) {
                commands.push(mActionBinding[pair.second]);
            }
        }
    };

    void assignKey(InputAction action, sf::Keyboard::Key key) {
        mKeyBinding[key] = action;
    }

    sf::Keyboard::Key getAssignedKey(InputAction action) {
        for (auto &pair : mKeyBinding) {
            if (pair.second == action) {
                return pair.first;
            }
        }
        return sf::Keyboard::Key::Unknown;
    }

  private:
    void initializeActions() {
        float playerSpeed = 200.f;

        mActionBinding[InputAction::moveLeft].action =
            derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
        mActionBinding[InputAction::moveRight].action =
            derivedAction<Aircraft>(AircraftMover(playerSpeed, 0.f));
        mActionBinding[InputAction::moveDown].action =
            derivedAction<Aircraft>(AircraftMover(0.f, playerSpeed));
        mActionBinding[InputAction::moveUp].action =
            derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
    }

    bool isLiveAction(InputAction action) {
        switch (action) {
        case InputAction::moveLeft:
        case InputAction::moveRight:
        case InputAction::moveDown:
        case InputAction::moveUp:
            return true;
        default:
            return false;
        }
    }

  private:
    std::map<sf::Keyboard::Key, InputAction> mKeyBinding;
    std::map<InputAction, Command> mActionBinding;
};
