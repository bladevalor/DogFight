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
