#pragma once

#include "Engine/CommandQueue.hpp"
#include "Engine/Player.hpp"
#include "Engine/World.hpp"
#include "Game/State.hpp"
#include "Game/StateIDs.hpp"
#include <SFML/Window/Event.hpp>

class GameState : public State {
  public:
    GameState(StateStack &stack, Context context)
        : State(stack, context), mWorld(*context.window, *context.fonts),
          mPlayer(*context.player) {
        mPlayer.setMissionStatus(MissionStatus::MissionRunning);
    }

    virtual void draw() { mWorld.draw(); }

    virtual bool update(sf::Time dt) {
        mWorld.update(dt);

        if (!mWorld.hasAlivePlayer()) {
            mPlayer.setMissionStatus(MissionStatus::MissionFailure);
            requestStackPush(StateID::GameOver);
        } else if (mWorld.hasPlayerReachedEnd()) {
            mPlayer.setMissionStatus(MissionStatus::MissionSuccess);
            requestStackPush(StateID::GameOver);
        }

        CommandQueue &commands = mWorld.getCommandQueue();
        mPlayer.handleLiveInput(commands);

        return true;
    }

    virtual bool handleEvent(const sf::Event &event) {
        CommandQueue &commands = mWorld.getCommandQueue();

        const auto *key        = event.getIf<sf::Event::KeyReleased>();
        if (key) {
            mPlayer.handleEvent(key->code, commands);

            if (key->code == sf::Keyboard::Key::Escape) {
                requestStackPush(StateID::Pause);
            }
        }

        return true;
    }

  private:
    World mWorld;
    Player &mPlayer;
};
