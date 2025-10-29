#include "Game/Aircraft.hpp"
#include "Engine/CommandQueue.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Pickup.hpp"
#include "Engine/Projectile.hpp"
#include "Engine/Resource.hpp"
#include "Engine/SceneNode.hpp"
#include "Engine/Utility.hpp"
#include "Engine/category.hpp"
#include "Game/DataTables.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

std::map<Aircraft_t, AircraftData> aircraftDataTable = initializeAircraftData();

Aircraft::Aircraft(Aircraft_t type, const TextureHolder &textures,
                   const FontHolder &fonts)
    : Entity(aircraftDataTable[type].hitpoints), mAircraftType(type),
      mAircraftSprite(textures.get(aircraftDataTable[type].texture)),
      mHealthDisplay(), mTravelledDistance(0.f), mDirectionIndex(0),
      mIsMarkedForRemoval(false), mDropPickupCommand(), mIsFiring(false),
      mReloadingTimer(sf::Time::Zero), mFireCommand(), mShotsPerSecond(1),
      mImpactZone(1), mIsLaunchingMissile(false), mMissileCommand(),
      mMissileDisplay(), mMissileAmmo(2) {
    centerOrigin(mAircraftSprite);

    mFireCommand.category = GameObjectCategory::Scene;
    mFireCommand.action   = [this, &textures](SceneNode &node, sf::Time) {
        createBullets(node, textures);
    };

    mMissileCommand.category = GameObjectCategory::Scene;
    mMissileCommand.action   = [this, &textures](SceneNode &node, sf::Time) {
        createProjectile(node, Projectile_t::Missile, 0.f, 0.5f, textures);
    };

    mDropPickupCommand.category = GameObjectCategory::Scene;
    mDropPickupCommand.action   = [this, &textures](SceneNode &node, sf::Time) {
        createPickup(node, textures);
    };

    std::unique_ptr<TextNode> healthDisplay =
        std::make_unique<TextNode>(fonts, "");
    mHealthDisplay = healthDisplay.get();
    addChild(std::move(healthDisplay));

    if (getCategory() == GameObjectCategory::PlayerAircraft) {
        std::unique_ptr<TextNode> missileDisplay =
            std::make_unique<TextNode>(fonts, "");
        missileDisplay->setPosition({0, 70});
        mMissileDisplay = missileDisplay.get();
        addChild(std::move(missileDisplay));
    }
};

void Aircraft::drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const {
    target.draw(mAircraftSprite, states);
}

GameObjectCategory Aircraft::getCategory() const {
    switch (mAircraftType) {
    case Aircraft_t::Eagle:
        return GameObjectCategory::PlayerAircraft;
    default:
        return GameObjectCategory::EnemyAircraft;
    }
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue &commands) {
    if (isDestroyed()) {
        checkPickupDrop(commands);

        mIsMarkedForRemoval = true;
        return;
    }

    // check if Bullets or Missiles are being fired
    checkProjectileLaunch(dt, commands);

    // update enemy movements
    updateMovementPattern(dt);
    Entity::updateCurrent(dt, commands);

    // update visible metrics;
    updateTexts();
}

void Aircraft::updateTexts() {
    mHealthDisplay->setString(std::to_string(getHintpoints()) + " HP");
    mHealthDisplay->setPosition({0.f, 50.f});
    mHealthDisplay->setRotation(-getRotation());

    if (mMissileDisplay) {
        if (mMissileAmmo == 0) {
            mMissileDisplay->setString("");
        } else {
            mMissileDisplay->setString("M: " + std::to_string(mMissileAmmo));
        }
    }
}

float Aircraft::getMaxSpeed() const {
    return aircraftDataTable[mAircraftType].speed;
}

void Aircraft::updateMovementPattern(sf::Time dt) {
    const std::vector<Direction> &directions =
        aircraftDataTable[mAircraftType].directions;
    if (!directions.empty()) {
        float travelDistance = directions[mDirectionIndex].distance;
        if (mTravelledDistance > travelDistance) {
            mDirectionIndex    = (mDirectionIndex + 1) % directions.size();
            mTravelledDistance = 0.f;
        }

        sf::Angle radians =
            sf::radians(directions[mDirectionIndex].angle) + sf::degrees(90.f);
        float vx = getMaxSpeed() + std::cos(radians.asRadians());
        float vy = getMaxSpeed() + std::sin(radians.asRadians());
        setVelocity({vx, vy});
        mTravelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}

void Aircraft::fire() {
    if (aircraftDataTable[mAircraftType].reloadingTime != sf::Time::Zero) {
        mIsFiring = true;
    }
}

void Aircraft::launchMissile() {
    if (mMissileAmmo > 0) {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}

void Aircraft::checkPickupDrop(CommandQueue &commands) {
    if (!isAllied() && randomInt(2) == 0) {
        commands.push(mDropPickupCommand);
    }
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue &commands) {
    if (!isAllied()) {
        fire();
    }

    if (mIsFiring && mReloadingTimer <= sf::Time::Zero) {
        commands.push(mFireCommand);
        mReloadingTimer += aircraftDataTable[mAircraftType].reloadingTime /
                           (mShotsPerSecond + 1.f);
        mIsFiring = false;
    } else if (mReloadingTimer > sf::Time::Zero) {
        mReloadingTimer -= dt;
    }

    if (mIsLaunchingMissile) {
        commands.push(mMissileCommand);
        mIsLaunchingMissile = false;
    }
}

void Aircraft::createProjectile(SceneNode &node, Projectile_t proj_type,
                                float xoffset, float yoffset,
                                const TextureHolder &textures) const {
    std::unique_ptr<Projectile> projectile =
        std::make_unique<Projectile>(proj_type, textures);

    sf::Vector2f offset(xoffset * mAircraftSprite.getGlobalBounds().size.x,
                        yoffset * mAircraftSprite.getGlobalBounds().size.y);
    sf::Vector2f velocity(0, projectile->getMaxSpeed());

    float sign = isAllied() ? -1.f : 1.f;

    projectile->setPosition(getWorldPosition() + offset * sign);
    projectile->setVelocity(velocity * sign);

    node.addChild(std::move(projectile));
};

void Aircraft::createBullets(SceneNode &node,
                             const TextureHolder &textures) const {
    Projectile_t proj_type =
        isAllied() ? Projectile_t::AlliedBullet : Projectile_t::EnemyBullet;

    switch (mImpactZone) {
    case 1:
        createProjectile(node, proj_type, 0.f, 0.5f, textures);
        break;
    case 2:
        createProjectile(node, proj_type, -0.33f, 0.33f, textures);
        createProjectile(node, proj_type, 0.33f, 0.33f, textures);
        break;
    case 3:
        createProjectile(node, proj_type, -0.5f, 0.33f, textures);
        createProjectile(node, proj_type, 0.f, 0.5f, textures);
        createProjectile(node, proj_type, 0.5f, 0.33f, textures);
        break;
    }
}

bool Aircraft::isAllied() const { return mAircraftType == Aircraft_t::Eagle; }

sf::FloatRect Aircraft::getBoundingBox() const {
    return getWorldTransform().transformRect(mAircraftSprite.getGlobalBounds());
}

bool Aircraft::isMarkedForRemoval() const { return mIsMarkedForRemoval; }

void Aircraft::collectMissiles(int new_missiles) {
    mMissileAmmo += new_missiles;
}

void Aircraft::increaseImapctZone() {
    if (mImpactZone < 3) {
        ++mImpactZone;
    }
}

void Aircraft::increaseFireRate() {
    if (mShotsPerSecond < 10) {
        ++mShotsPerSecond;
    }
}

void Aircraft::createPickup(SceneNode &node,
                            const TextureHolder &textures) const {
    Pickup_t pickup_type =
        static_cast<Pickup_t>(randomInt((size_t)Pickup_t::count));

    std::unique_ptr<Pickup> pickup =
        std::make_unique<Pickup>(pickup_type, textures);

    pickup->setPosition(getWorldPosition());
    pickup->setVelocity({0.f, 1.f});
    node.addChild(std::move(pickup));
}
