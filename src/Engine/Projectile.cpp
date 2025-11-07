#include "Engine/Projectile.hpp"
#include "Engine/CommandQueue.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Resource.hpp"
#include "Engine/Utility.hpp"
#include "Engine/category.hpp"
#include "Game/DataTables.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cmath>
#include <map>

std::map<Projectile_t, ProjectileData> ProjectileDataTable =
    initializeProjectileData();

Projectile::Projectile(Projectile_t type, const TextureHolder &textures)
    : Entity(1), mProjectileType(type), mTargetDirection(),
      mProjectileSprite(textures.get(ProjectileDataTable[type].texture)) {
    centerOrigin(mProjectileSprite);
}

void Projectile::drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) const {
    target.draw(mProjectileSprite, states);
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue &commands) {
    if (isGuided()) {
        // INFO: heat seeking algorithm
        const float projectile_acceleration = 200.f;
        float velocity_scalar =
            getMaxSpeed() * (projectile_acceleration * dt.asSeconds());

        sf::Vector2f projectile_direction =
            unitVector(mTargetDirection + getVelocity());
        sf::Vector2f projectile_velocity =
            projectile_direction * velocity_scalar;

        float projectile_angle =
            std::atan2(projectile_velocity.y, projectile_velocity.x);

        setRotation(sf::radians(projectile_angle) + sf::degrees(90.f));
        setVelocity(projectile_velocity);
    }

    Entity::updateCurrent(dt, commands);
}

bool Projectile::isGuided() const {
    return mProjectileType == Projectile_t::Missile;
}

void Projectile::guideTowards(sf::Vector2f position) {
    assert(isGuided());
    mTargetDirection = unitVector(position - getWorldPosition());
}

int Projectile::getDamage() const {
    return ProjectileDataTable[mProjectileType].damage;
}

float Projectile::getMaxSpeed() const {
    return ProjectileDataTable[mProjectileType].speed;
}

GameObjectCategory Projectile::getCategory() const {
    if (mProjectileType == Projectile_t::EnemyBullet) {
        return GameObjectCategory::EnemyProjectile;
    } else {
        return GameObjectCategory::AlliedProjectile;
    }
}

sf::FloatRect Projectile::getBoundingBox() const {
    return getWorldTransform().transformRect(
        mProjectileSprite.getGlobalBounds());
}
