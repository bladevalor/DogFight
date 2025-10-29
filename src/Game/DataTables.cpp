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
