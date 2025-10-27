#pragma once

#include "Engine/Resource.hpp"
#include "Game/Aircraft.hpp"
#include <map>

struct AircraftData {
    int hitpoints;
    float speed;
    TextureId texture;
};

std::map<Aircraft_t, AircraftData> initializeAircraftData() {
    std::map<Aircraft_t, AircraftData> data;

    data[Aircraft_t::Eagle].hitpoints  = 100;
    data[Aircraft_t::Eagle].speed      = 200.f;
    data[Aircraft_t::Eagle].texture    = TextureId::Eagle;

struct PickupData {
    TextureId texture;
    std::function<void(Aircraft &)> action;
};

std::map<Aircraft_t, AircraftData> initializeAircraftData();
std::map<Projectile_t, ProjectileData> initializeProjectileData();
std::map<Pickup_t, PickupData> initializePickupData();
