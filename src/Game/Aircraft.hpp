#pragma once

#include "Engine/Command.hpp"
#include "Engine/CommandQueue.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Projectile.hpp"
#include "Engine/Resource.hpp"
#include "Engine/SceneNode.hpp"
#include "Engine/TextNode.hpp"
#include "Engine/category.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <cstddef>
#include <vector>

enum class Aircraft_t { Eagle, Raptor, Avenger };
static std::vector<Aircraft_t> allAircraftTypes = {
    Aircraft_t::Eagle, Aircraft_t::Raptor, Aircraft_t::Avenger};

class Aircraft : public Entity {
  public:
    Aircraft(Aircraft_t type, const TextureHolder &textures,
             const FontHolder &fonts);

    GameObjectCategory getCategory() const override;
    sf::FloatRect getBoundingBox() const;
    float getMaxSpeed() const;
    bool isMarkedForRemoval() const override;
    bool isAllied() const;

    void collectMissiles(int missiles);
    void increaseFireRate();
    void increaseImapctZone();

    void fire();
    void launchMissile();

  private:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) const override;

    void updateMovementPattern(sf::Time dt);
    void updateTexts();

    void checkProjectileLaunch(sf::Time dt, CommandQueue &commands);
    void checkPickupDrop(CommandQueue &commands);

    void createPickup(SceneNode &node, const TextureHolder &textures) const;
    void createBullets(SceneNode &node, const TextureHolder &textures) const;
    void createProjectile(SceneNode &node, Projectile_t proj_type,
                          float xoffset, float yoffset,
                          const TextureHolder &textures) const;

  private:
    Aircraft_t mAircraftType;
    sf::Sprite mAircraftSprite;
    TextNode *mHealthDisplay;

    float mTravelledDistance;
    std::size_t mDirectionIndex;
    bool mIsMarkedForRemoval;
    Command mDropPickupCommand;

    // bullet metrics
    bool mIsFiring;
    sf::Time mReloadingTimer;
    Command mFireCommand;
    int mShotsPerSecond;
    int mImpactZone;

    // missile metrics
    bool mIsLaunchingMissile;
    Command mMissileCommand;
    TextNode *mMissileDisplay;
    int mMissileAmmo;
};
