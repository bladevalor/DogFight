#pragma once

#include "Engine/CommandQueue.hpp"
#include "SceneNode.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cassert>

class Entity : public SceneNode {
  public:
    explicit Entity(int hitpoints) : mHintpoints(hitpoints) {}

  public:
    sf::Vector2f getVelocity() const { return mVelocity; }

    void setVelocity(double vx, double vy) {
        mVelocity.x = vx;
        mVelocity.y = vy;
    }

    void setVelocity(sf::Vector2f new_vel) { mVelocity = new_vel; }

    void accelerate(sf::Vector2f velocity) { mVelocity += velocity; }

    void accelerate(float vx, float vy) {
        mVelocity.x += vx;
        mVelocity.y += vy;
    }

    // virtual sf::FloatRect getBoundingBox() const;

  public:
    void repair(int hitpoints) {
        assert(hitpoints > 0);

        mHintpoints += hitpoints;
    }

    void damage(int hitpoints) {
        assert(hitpoints > 0);
        mHintpoints -= hitpoints;
    }

    void destroy() { mHintpoints = 0; }
    int getHintpoints() const { return mHintpoints; }
    bool isDestroyed() const { return mHintpoints <= 0; }

  protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &) {
        move(mVelocity * dt.asSeconds());
    }

  private:
    sf::Vector2f mVelocity;
    int mHintpoints;
};
