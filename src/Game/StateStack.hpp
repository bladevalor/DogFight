#pragma once

#include "Game/State.hpp"
#include "Game/StateIDs.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <map>
#include <memory>
#include <vector>

enum class StackAction { Push, Pop, Clear };

class StateStack {
  public:
    explicit StateStack(State::Context context);

    template <typename StateObject> void registerState(StateID id) {
        mFactories[id] = [this]() {
            return std::make_unique<StateObject>(*this, mContext);
        };
    }

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event &event);

    void pushState(StateID id);
    void popState();
    void clearState();

    bool isEmpty() const;

  private:
    State::StatePointer createState(StateID id);
    void applyPendingChanges();

  private:
    struct PendingChange {
        explicit PendingChange(StackAction action, StateID id = StateID::None);

        StackAction action;
        StateID id;
    };

  private:
    std::vector<State::StatePointer> mStack;
    std::vector<PendingChange> mPendingList;
    State::Context mContext;
    std::map<StateID, std::function<State::StatePointer()>> mFactories;
};
