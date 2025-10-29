#pragma once

#include "Engine/Command.hpp"
#include "Engine/CommandQueue.hpp"
#include "Game/Aircraft.hpp"
#include "category.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cassert>
#include <functional>
#include <map>
#include <vector>

enum class MissionStatus { MissionRunning, MissionSuccess, MissionFailure };

enum class InputAction {
    moveLeft,
    moveRight,
    moveDown,
    moveUp,
    Fire,
    LaunchMissile
};
static std::vector<InputAction> allInputActions = {
    InputAction::moveLeft, InputAction::moveRight, InputAction::moveDown,
    InputAction::moveUp,   InputAction::Fire,      InputAction::LaunchMissile};

struct AircraftMover {
    AircraftMover(float vx, float vy) : velocity({vx, vy}) {};

    void operator()(Aircraft &aircraft, sf::Time) const {
        aircraft.accelerate(velocity);
    };
    sf::Vector2f velocity;
};

class Player {
  public:
    Player() : mCurrentMissionStatus(MissionStatus::MissionRunning) {
        mKeyBinding[sf::Keyboard::Key::Left]  = InputAction::moveLeft;
        mKeyBinding[sf::Keyboard::Key::Right] = InputAction::moveRight;
        mKeyBinding[sf::Keyboard::Key::Down]  = InputAction::moveDown;
        mKeyBinding[sf::Keyboard::Key::Up]    = InputAction::moveUp;
        mKeyBinding[sf::Keyboard::Key::Space] = InputAction::Fire;
        mKeyBinding[sf::Keyboard::Key::M]     = InputAction::LaunchMissile;

        initializeActions();

        for (auto &pair : mActionBinding) {
            pair.second.category = GameObjectCategory::PlayerAircraft;
        }
    };

    void handleEvent(sf::Keyboard::Key key, CommandQueue &commands) {
        auto found = mKeyBinding.find(key);
        if (found != mKeyBinding.end() && !isLiveAction(found->second)) {
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
        // delete all keys mapped to the current "action"
        std::erase_if(mKeyBinding,
                      [&](const auto &pair) { return pair.second == action; });

        // insert new KeyBinding
        mKeyBinding[key] = action;
    }

    sf::Keyboard::Key getAssignedKey(InputAction action) {
        auto found = std::ranges::find_if(mKeyBinding, [&](const auto &pair) {
            return pair.second == action;
        });
        assert(found != mKeyBinding.end());

        return found->first;
    }

    MissionStatus getMissionStatus() const { return mCurrentMissionStatus; }

    void setMissionStatus(MissionStatus status) {
        mCurrentMissionStatus = status;
    }

  private:
    void initializeActions() {
        mActionBinding[InputAction::moveLeft].action =
            derivedAction<Aircraft>(AircraftMover(-1, 0.f));
        mActionBinding[InputAction::moveRight].action =
            derivedAction<Aircraft>(AircraftMover(1, 0.f));
        mActionBinding[InputAction::moveDown].action =
            derivedAction<Aircraft>(AircraftMover(0.f, 1));
        mActionBinding[InputAction::moveUp].action =
            derivedAction<Aircraft>(AircraftMover(0.f, -1));
        mActionBinding[InputAction::Fire].action =
            derivedAction<Aircraft>([](Aircraft &a, sf::Time) { a.fire(); });
        mActionBinding[InputAction::LaunchMissile].action =
            derivedAction<Aircraft>(
                [](Aircraft &a, sf::Time) { a.launchMissile(); });
    }

    bool isLiveAction(InputAction action) {
        switch (action) {
        case InputAction::moveLeft:
        case InputAction::moveRight:
        case InputAction::moveDown:
        case InputAction::moveUp:
        case InputAction::Fire:
            return true;
        default:
            return false;
        }
    }

  private:
    std::map<sf::Keyboard::Key, InputAction> mKeyBinding;
    std::map<InputAction, Command> mActionBinding;
    MissionStatus mCurrentMissionStatus;
};
