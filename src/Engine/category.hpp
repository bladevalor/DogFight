#pragma once

enum class GameObjectCategory {
    None             = 0,
    Scene            = 1 << 0,
    PlayerAircraft   = 1 << 1,
    AlliedAircraft   = 1 << 2,
    EnemyAircraft    = 1 << 3,
    Pickup           = 1 << 4,
    AlliedProjectile = 1 << 5,
    EnemyProjectile  = 1 << 6,

    Projectile       = AlliedProjectile | EnemyProjectile,
};
