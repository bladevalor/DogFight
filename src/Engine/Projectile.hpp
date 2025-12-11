#pragma once

#include "Engine/CommandQueue.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Resource.hpp"
#include "Engine/category.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>

enum class Projectile_t { AlliedBullet, EnemyBullet, Missile };

class Projectile : public Entity {
  public:
    Projectile(Projectile_t type, const TextureHolder &textures);

    void guideTowards(sf::Vector2f position);
    bool isGuided() const;

    virtual GameObjectCategory getCategory() const;
    virtual sf::FloatRect getBoundingBox() const;
    float getMaxSpeed() const;
    int getDamage() const;

  private:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) const;

  private:
    Projectile_t mProjectileType;
    sf::Sprite mProjectileSprite;
    sf::Vector2f mTargetDirection;
};
