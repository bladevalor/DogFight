#include "Engine/World.hpp"
#include "Engine/Pickup.hpp"
#include "Engine/SpriteNode.hpp"
#include "Engine/category.hpp"
#include "Game/Aircraft.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>

bool matchesCategories(SceneNode::CollisionPair &colliders,
                       GameObjectCategory type_a, GameObjectCategory type_b) {
    GameObjectCategory element_a = colliders.first->getCategory();
    GameObjectCategory element_b = colliders.second->getCategory();

    if (type_a == element_a && type_b == element_b) {
        return true;
    } else if (type_a == element_b && type_b == element_a) {
        std::swap(colliders.first, colliders.second);
        return true;
    } else {
        return false;
    }
}

World::World(sf::RenderWindow &window, FontHolder &fonts)
    : mFonts(fonts), mWindow(window), mWorldView(window.getDefaultView()),
      mWorldBounds({0, 0},
                   {
                       (float)window.getSize().x, // width
                       2000.f                     // height
                   }),
      mSpawnPosition(window.getSize().x * 0.5f,
                     mWorldBounds.size.y - mWorldView.getSize().y * 0.5f),
      mTextures(), mWorldSceneGraph(), mWorldSceneLayers(), mEnemySpawnPoints(),
      mActiveEnemies(), mScrollSpeed(-50.f), mPlayerAircraft(nullptr) {
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void World::handleCollisions() {
    std::set<SceneNode::CollisionPair> collisionMatches;
    mWorldSceneGraph.checkSceneCollision(mWorldSceneGraph, collisionMatches);

    for (auto pair : collisionMatches) {
        // INFO: SAFEGUARD from "stale" objects that have not yet been removed
        // from the scene
        if (pair.first->isMarkedForRemoval() ||
            pair.second->isMarkedForRemoval()) {
            continue;
        }

        if (matchesCategories(pair, GameObjectCategory::PlayerAircraft,
                              GameObjectCategory::EnemyAircraft)) {
            auto &player = static_cast<Aircraft &>(*pair.first);
            auto &enemy  = static_cast<Aircraft &>(*pair.second);

            player.damage(enemy.getHintpoints());
            enemy.destroy();
        } else if (matchesCategories(pair, GameObjectCategory::PlayerAircraft,
                                     GameObjectCategory::Pickup)) {
            auto &player = static_cast<Aircraft &>(*pair.first);
            auto &pickup = static_cast<Pickup &>(*pair.second);

            pickup.apply(player);
            pickup.destroy();
        } else if (matchesCategories(pair, GameObjectCategory::EnemyAircraft,
                                     GameObjectCategory::AlliedProjectile) ||
                   matchesCategories(pair, GameObjectCategory::PlayerAircraft,
                                     GameObjectCategory::EnemyProjectile)) {
            auto &aircraft   = static_cast<Aircraft &>(*pair.first);
            auto &projectile = static_cast<Projectile &>(*pair.second);

            aircraft.damage(projectile.getDamage());
            projectile.destroy();
        }
    }
}

void World::guideMissiles() {
    // collect active enemies
    Command enemyCollector;
    enemyCollector.category = GameObjectCategory::EnemyAircraft;
    enemyCollector.action =
        derivedAction<Aircraft>([this](Aircraft &enemy, sf::Time) {
            if (!enemy.isDestroyed()) {
                mActiveEnemies.push_back(&enemy);
            }
        });

    // seek closest enemy
    Command enemySeeker;
    enemySeeker.category = GameObjectCategory::AlliedProjectile;
    enemySeeker.action =
        derivedAction<Projectile>([this](Projectile &missile, sf::Time) {
            if (!missile.isGuided()) {
                return;
            }

            float minDistance      = std::numeric_limits<float>::max();
            Aircraft *closestEnemy = nullptr;

            for (auto enemy : mActiveEnemies) {
                float enemyDistance = distance(missile, *enemy);
                if (enemyDistance < minDistance) {
                    closestEnemy = enemy;
                    minDistance  = enemyDistance;
                }
            }

            if (closestEnemy) {
                missile.guideTowards(closestEnemy->getWorldPosition());
            }
        });

    mGlobalCommandQueue.push(enemyCollector);
    mGlobalCommandQueue.push(enemySeeker);
    mActiveEnemies.clear();
}

void World::addEnemies() {
    addEnemy(Aircraft_t::Raptor, 0, 500);
    addEnemy(Aircraft_t::Raptor, 0, 1000);
    addEnemy(Aircraft_t::Raptor, +100, 1100);
    addEnemy(Aircraft_t::Raptor, -100, 1100);
    addEnemy(Aircraft_t::Avenger, -70, 1400);
    addEnemy(Aircraft_t::Avenger, -70, 1600);
    addEnemy(Aircraft_t::Avenger, 70, 1400);
    addEnemy(Aircraft_t::Avenger, 70, 1600);

    std::ranges::sort(mEnemySpawnPoints, [](SpawnPoint lhs, SpawnPoint rhs) {
        return lhs.position.y < rhs.position.y;
    });
}

void World::addEnemy(Aircraft_t type, float x, float y) {
    mEnemySpawnPoints.push_back(
        SpawnPoint(type, {mSpawnPosition.x + x, mSpawnPosition.y - y}));
}

void World::spawnEnemies() {
    while (!mEnemySpawnPoints.empty() &&
           mEnemySpawnPoints.back().position.y >
               getBattlefieldBounds().position.y) {
        SpawnPoint spawn = mEnemySpawnPoints.back();

        std::unique_ptr<Aircraft> enemy =
            std::make_unique<Aircraft>(spawn.type, mTextures, mFonts);
        enemy->setPosition(spawn.position);
        enemy->setRotation(sf::degrees(180));

        mWorldSceneLayers[(size_t)Layer::Air]->addChild(std::move(enemy));

        mEnemySpawnPoints.pop_back();
    }
}

sf::FloatRect World::getViewBounds() const {

    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f,
                         mWorldView.getSize());
}

void World::adoptPlayerPosition() {
    // Keep player's position inside the screen bounds, at least
    // borderDistance units from the border
    sf::FloatRect viewBounds   = getViewBounds();
    const float borderDistance = 40.f;

    sf::Vector2f position      = mPlayerAircraft->getPosition();
    position.x = std::max(position.x, viewBounds.position.x + borderDistance);
    position.x = std::min(position.x, viewBounds.position.x +
                                          viewBounds.size.x - borderDistance);
    position.y = std::max(position.y, viewBounds.position.y + borderDistance);
    position.y = std::min(position.y, viewBounds.position.y +
                                          viewBounds.size.y - borderDistance);
    mPlayerAircraft->setPosition(position);
}

void World::adoptPlayerVelocity() {
    sf::Vector2f vel = mPlayerAircraft->getVelocity();
    if (vel.x != 0.f && vel.y != 0.f) {
        mPlayerAircraft->setVelocity(vel /
                                     std::sqrt(2.f)); // fix diagonal velocity
    }
    mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

void World::buildScene() {
    // initialize layers
    for (size_t i = 0; i < (size_t)Layer::LayerCount; i++) {
        GameObjectCategory gameObject = (static_cast<Layer>(i) == Layer::Air)
                                            ? GameObjectCategory::Scene
                                            : GameObjectCategory::None;
        SceneNode::SceneNodePointer layer =
            std::make_unique<SceneNode>(gameObject);
        mWorldSceneLayers[i] = layer.get();
        mWorldSceneGraph.addChild(std::move(layer));
    }

    // tile Background
    sf::Texture &backgroundTexture = mTextures.get(TextureId::Desert);
    sf::IntRect textureRect(mWorldBounds);
    backgroundTexture.setRepeated(true);

    // Add Background sprite to scene
    std::unique_ptr<SpriteNode> backgroundSprite =
        std::make_unique<SpriteNode>(backgroundTexture, textureRect);
    backgroundSprite->setPosition(mWorldBounds.position);
    mWorldSceneLayers[(int)Layer::Background]->addChild(
        std::move(backgroundSprite));

    // add Player's Aircraft
    std::unique_ptr<Aircraft> player =
        std::make_unique<Aircraft>(Aircraft_t::Eagle, mTextures, mFonts);
    mPlayerAircraft = player.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mWorldSceneLayers[(int)Layer::Air]->addChild(std::move(player));

    // add Enemy Aircraft
    addEnemies();
}

void World::update(sf::Time dt) {
    mWorldView.move({0.f, mScrollSpeed * dt.asSeconds()});
    mPlayerAircraft->setVelocity({0.f, 0.f});

    destroyEntitiesOutsideView();
    guideMissiles();

    // forward commands to the Scene Graph
    while (!mGlobalCommandQueue.isEmpty()) {
        mWorldSceneGraph.onCommand(mGlobalCommandQueue.pop(), dt);
    }
    adoptPlayerVelocity();

    handleCollisions();
    mWorldSceneGraph.removeWrecks();
    spawnEnemies();

    mWorldSceneGraph.update(dt, mGlobalCommandQueue);
    adoptPlayerPosition();
};

void World::draw() {
    mWindow.setView(mWorldView);
    mWindow.draw(mWorldSceneGraph);
};

CommandQueue &World::getCommandQueue() { return mGlobalCommandQueue; }

sf::FloatRect World::getBattlefieldBounds() const {
    sf::FloatRect battlefield = getViewBounds();
    battlefield.position.y -= 100.f;
    battlefield.size.y += 100.f;

    return battlefield;
}

void World::destroyEntitiesOutsideView() {
    Command killObject;

    killObject.category =
        GameObjectCategory::EnemyAircraft | CompositeGameObject::Projectile;

    killObject.action = derivedAction<Entity>([this](Entity &e, sf::Time) {
        if (!getBattlefieldBounds()
                 .findIntersection(e.getBoundingBox())
                 .has_value()) {
            e.destroy();
        }
    });

    mGlobalCommandQueue.push(killObject);
}

void World::loadTextures() {
    mTextures.load(TextureId::Eagle, "assets/Eagle.png");
    mTextures.load(TextureId::Raptor, "assets/Raptor.png");
    mTextures.load(TextureId::Desert, "assets/Desert.png");
    mTextures.load(TextureId::Avenger, "assets/Avenger.png");

    mTextures.load(TextureId::Bullet, "assets/Bullet.png");
    mTextures.load(TextureId::Missile, "assets/Missile.png");

    mTextures.load(TextureId::HealthBoost, "assets/HealthRefill.png");
    mTextures.load(TextureId::MissilePickup, "assets/MissileRefill.png");
    mTextures.load(TextureId::FireSpread, "assets/FireSpread.png");
    mTextures.load(TextureId::FireRate, "assets/FireRate.png");
};

bool World::hasAlivePlayer() const {
    return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const {
    return !mWorldBounds.contains(mPlayerAircraft->getPosition());
}
