#pragma once

#include <SFML/System.hpp>
#include <chrono>
#include <mutex>
#include <thread>

class ParallelTask {
  public:
    ParallelTask() : mFinished(false), mThread(&ParallelTask::runTask, this) {}

    void execute() {
        mFinished = false;
        mElapsedTime.restart();
        mThread.detach();
    }

    bool isFinished() {
        std::lock_guard<std::mutex> lock(mMutex);
        return mFinished;
    }

    float getCompletion() {
        std::lock_guard<std::mutex> lock(mMutex);
        return mElapsedTime.getElapsedTime().asSeconds() / 10.f;
    }

  private:
    void runTask() {
        // dummy task taking 10 seconds long
        bool ended = false;
        while (!ended) {
            {
                std::lock_guard<std::mutex> lock(mMutex);
                if (mElapsedTime.getElapsedTime().asSeconds() >= 10.f) {
                    ended = true;
                }
            }

            if (!ended) {
                // sleep for 10 milliseconds to let the CPU rest
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mFinished = true;
        }
    }

  private:
    std::thread mThread;
    bool mFinished;
    sf::Clock mElapsedTime;
    std::mutex mMutex;
};
