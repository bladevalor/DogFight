#pragma once

#include "Command.hpp"
#include <queue>

class CommandQueue {
  public:
    CommandQueue() {};

    // delete copy operations
    CommandQueue(const CommandQueue &)            = delete;
    CommandQueue &operator=(const CommandQueue &) = delete;

    void push(const Command &command) { mQueue.push(command); }

    Command pop() {
        Command command = mQueue.front();
        mQueue.pop();
        return command;
    }

    bool isEmpty() const { return mQueue.empty(); }

  private:
    std::queue<Command> mQueue;
};
