#include "Game/StateStack.hpp"

StateStack::StateStack(State::Context context)
    : mStack(), mPendingList(), mContext(context), mFactories() {}

void StateStack::update(sf::Time dt) {
    for (auto iter = mStack.rbegin(); iter < mStack.rend(); iter++) {
        if (!(*iter)->update(dt)) {
            break;
        }
    }

    applyPendingChanges();
}

void StateStack::draw() {
    // draw all states from bottom to top
    for (auto &state : mStack) {
        state->draw();
    }
}

void StateStack::handleEvent(const sf::Event &event) {
    // iterate from recent to oldest State
    for (auto iter = mStack.rbegin(); iter < mStack.rend(); ++iter) {
        if (!(*iter)->handleEvent(event)) {
            return;
        }
    }
    applyPendingChanges();
};

void StateStack::pushState(StateID id) {
    mPendingList.push_back(PendingChange(StackAction::Push, id));
}

void StateStack::popState() {
    mPendingList.push_back(PendingChange(StackAction::Pop));
}

void StateStack::clearState() {
    mPendingList.push_back(PendingChange(StackAction::Clear));
}

bool StateStack::isEmpty() const { return mStack.empty(); }

State::StatePointer StateStack::createState(StateID id) {
    auto found = mFactories.find(id);
    assert(found != mFactories.end());

    return found->second();
}

void StateStack::applyPendingChanges() {
    for (PendingChange change : mPendingList) {
        switch (change.action) {
        case StackAction::Push:
            mStack.push_back(createState(change.id));
            break;
        case StackAction::Pop:
            mStack.pop_back();
            break;
        case StackAction::Clear:
            mStack.clear();
            break;
        default:
            throw std::runtime_error("ERROR:  Unknown StackAction");
        }
    }
    mPendingList.clear();
}

StateStack::PendingChange::PendingChange(StackAction action, StateID id)
    : action(action), id(id) {}
