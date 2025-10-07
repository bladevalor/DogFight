#pragma once

#include "SceneNode.hpp"
#include <SFML/Graphics.hpp>

class Entity : public SceneNode {
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

  private:
    void updateCurrent(sf::Time dt) override {
        move(mVelocity * dt.asSeconds());
    }

  private:
    sf::Vector2f mVelocity;
};
