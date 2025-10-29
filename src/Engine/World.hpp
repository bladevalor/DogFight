#pragma once

#include "Engine/Command.hpp"
#include "Engine/CommandQueue.hpp"
#include "Engine/Resource.hpp"
#include "Engine/SceneNode.hpp"
#include "Game/Aircraft.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <cstdio>
#include <vector>

enum class Layer { Background, Air, LayerCount };

class World {
  public:
    explicit World(sf::RenderWindow &window, FontHolder &fonts);

    World(const World &)            = delete; // delete copy constructor
    World &operator=(const World &) = delete; // delete copy assignment

    void update(sf::Time dt);
    void draw();

    CommandQueue &getCommandQueue();

    bool hasAlivePlayer() const;
    bool hasPlayerReachedEnd() const;

  private:
    void loadTextures();
    void adoptPlayerVelocity();
    void adoptPlayerPosition();
    void handleCollisions();

    void buildScene();
    void addEnemies();
    void addEnemy(Aircraft_t type, float x, float y);
    void spawnEnemies();
    void destroyEntitiesOutsideView();
    void guideMissiles();
    sf::FloatRect getViewBounds() const;
    sf::FloatRect getBattlefieldBounds() const;

  private:
    struct SpawnPoint {
        SpawnPoint(Aircraft_t type, sf::Vector2f pos)
            : type(type), position(pos) {}

        Aircraft_t type;
        sf::Vector2f position;
    };

  private:
    sf::RenderWindow &mWindow;
    sf::View mWorldView;
    TextureHolder mTextures;
    FontHolder &mFonts;

    SceneNode mWorldSceneGraph;
    std::array<SceneNode *, (size_t)Layer::LayerCount> mWorldSceneLayers;
    CommandQueue mGlobalCommandQueue;

    sf::FloatRect mWorldBounds;
    sf::Vector2f mSpawnPosition;
    float mScrollSpeed;
    Aircraft *mPlayerAircraft;

    std::vector<SpawnPoint> mEnemySpawnPoints;
    std::pmr::vector<Aircraft *> mActiveEnemies;
};
