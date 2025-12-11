#include "Game/DataTables.hpp"
#include "Engine/Projectile.hpp"
#include "Engine/Resource.hpp"
#include "Game/Aircraft.hpp"
#include <SFML/System/Time.hpp>
#include <functional>
#include <map>

std::map<Aircraft_t, AircraftData> initializeAircraftData() {
    std::map<Aircraft_t, AircraftData> data;

    data[Aircraft_t::Eagle].hitpoints     = 100;
    data[Aircraft_t::Eagle].speed         = 200.f;
    data[Aircraft_t::Eagle].texture       = TextureId::Eagle;
    data[Aircraft_t::Eagle].reloadingTime = sf::seconds(1);

    data[Aircraft_t::Raptor].hitpoints    = 20;
    data[Aircraft_t::Raptor].speed        = 80.f;
    data[Aircraft_t::Raptor].texture      = TextureId::Raptor;
    data[Aircraft_t::Raptor].directions.push_back(Direction(45, 80));
    data[Aircraft_t::Raptor].directions.push_back(Direction(-45, 160));
    data[Aircraft_t::Raptor].directions.push_back(Direction(45, 80));
    data[Aircraft_t::Raptor].reloadingTime =
        sf::milliseconds(500); // sf::Time::Zero;

    data[Aircraft_t::Avenger].hitpoints = 40;
    data[Aircraft_t::Avenger].speed     = 50.f;
    data[Aircraft_t::Avenger].texture   = TextureId::Avenger;
    data[Aircraft_t::Avenger].directions.push_back(Direction(45, 50));
    data[Aircraft_t::Avenger].directions.push_back(Direction(0, 50));
    data[Aircraft_t::Avenger].directions.push_back(Direction(-45, 100));
    data[Aircraft_t::Avenger].directions.push_back(Direction(45, 50));
    data[Aircraft_t::Avenger].reloadingTime = sf::seconds(2);

    return data;
}

std::map<Projectile_t, ProjectileData> initializeProjectileData() {
    std::map<Projectile_t, ProjectileData> data;

    data[Projectile_t::AlliedBullet].damage  = 10;
    data[Projectile_t::AlliedBullet].speed   = 300.f;
    data[Projectile_t::AlliedBullet].texture = TextureId::Bullet;

    data[Projectile_t::EnemyBullet].damage   = 10;
    data[Projectile_t::EnemyBullet].speed    = 300.f;
    data[Projectile_t::EnemyBullet].texture  = TextureId::Bullet;

    data[Projectile_t::Missile].damage       = 200;
    data[Projectile_t::Missile].speed        = 150.f;
    data[Projectile_t::Missile].texture      = TextureId::Missile;

    return data;
}

std::map<Pickup_t, PickupData> initializePickupData() {
    std::map<Pickup_t, PickupData> data;

    data[Pickup_t::HealthBoost].texture   = TextureId::HealthBoost;
    data[Pickup_t::HealthBoost].action    = [](Aircraft &a) { a.repair(25); };

    data[Pickup_t::MissilePickup].texture = TextureId::MissilePickup;
    data[Pickup_t::MissilePickup].action  = [](Aircraft &a) {
        a.collectMissiles(3);
    };
    // std::bind(&Aircraft::collectMissiles, std::placeholders::_1, 3);

    data[Pickup_t::FireSpread].texture = TextureId::FireSpread;
    data[Pickup_t::FireSpread].action  = [](Aircraft &a) {
        a.increaseImapctZone();
    };
    // std::bind(&Aircraft::increaseSpread, std::placeholders::_1);

    data[Pickup_t::FireRate].texture = TextureId::FireRate;
    data[Pickup_t::FireRate].action = [](Aircraft &a) { a.increaseFireRate(); };
    // std::bind(&Aircraft::increaseFireRate, std::placeholders::_1);

    return data;
}
