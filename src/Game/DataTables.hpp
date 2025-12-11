#pragma once

#include "Engine/Pickup.hpp"
#include "Engine/Projectile.hpp"
#include "Engine/Resource.hpp"
#include "Game/Aircraft.hpp"
#include <SFML/System/Time.hpp>
#include <functional>
#include <map>
#include <vector>

struct Direction {
    Direction(float angle, float distance) : angle(angle), distance(distance) {}

    float angle;
    float distance;
};

struct AircraftData {
    int hitpoints;
    float speed;
    TextureId texture;
    sf::Time reloadingTime;
    std::vector<Direction> directions;
};

struct ProjectileData {
    int damage;
    float speed;
    TextureId texture;
};

struct PickupData {
    TextureId texture;
    std::function<void(Aircraft &)> action;
};

std::map<Aircraft_t, AircraftData> initializeAircraftData();
std::map<Projectile_t, ProjectileData> initializeProjectileData();
std::map<Pickup_t, PickupData> initializePickupData();
