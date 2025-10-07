#pragma once

#include "Engine/CommandQueue.hpp"
#include "Engine/Resource.hpp"
#include "Engine/SceneNode.hpp"
#include "Engine/SpriteNode.hpp"
#include "Game/Aircraft.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <utility>

enum class Layer { Background, Air, LayerCount };

class World {
  public:
    World(sf::RenderWindow &window)
        : mWindow(window), mWorldView(window.getDefaultView()),
          mWorldBounds(
              {
                  0, // left
                  0  // top
              },
              {
                  (float)window.getSize().x, // width
                  2000.f                     // height
              }),
          mSpawnPosition(window.getSize().x / 2.f,
                         mWorldBounds.size.y - mWorldView.getSize().y / 2.f),
          mScrollSpeed(-50.f), mPlayerAircraft(nullptr) {
        loadTextures();
        buildScene();

        mWorldView.setCenter(mSpawnPosition);
    };
    World(const World &)            = delete; // delete copy constructor
    World &operator=(const World &) = delete; // delete copy assignment

    void update(sf::Time dt) {
        mWorldView.move({0.f, mScrollSpeed * dt.asSeconds()});
        mPlayerAircraft->setVelocity({0.f, 0.f});

        // forward commands to the Scene Graph
        while (!mCommandQueue.isEmpty()) {
            mSceneGraph.onCommand(mCommandQueue.pop(), dt);
        }
        adoptPlayerVelocity();

        mSceneGraph.update(dt);
        adoptPlayerPosition();
    };

    void draw() {
        mWindow.setView(mWorldView);
        mWindow.draw(mSceneGraph);
    };

    CommandQueue &getCommandQueue() { return mCommandQueue; }

  private:
    void loadTextures() {
        mTextures.load(TextureId::Eagle, "assets/Eagle.png");
        mTextures.load(TextureId::Raptor, "assets/Raptor.png");
        mTextures.load(TextureId::Desert, "assets/Desert.png");
    };

    void buildScene() {
        // initialize layers
        for (size_t i = 0; i < (size_t)Layer::LayerCount; i++) {
            SceneNode::SceneNodePointer layer(new SceneNode());
            mSceneLayers[i] = layer.get();
            mSceneGraph.addChild(std::move(layer));
        }

        // tile Background
        sf::Texture &backgroundTexture = mTextures.get(TextureId::Desert);
        sf::IntRect textureRect(mWorldBounds);
        backgroundTexture.setRepeated(true);

        // Add Background sprite to scene
        std::unique_ptr<SpriteNode> backgroundSprite(
            new SpriteNode(backgroundTexture, textureRect));
        backgroundSprite->setPosition(mWorldBounds.position);
        mSceneLayers[(int)Layer::Background]->addChild(
            std::move(backgroundSprite));

        // add Leading Aircraft
        std::unique_ptr<Aircraft> leader(
            new Aircraft(Aircraft_t::Eagle, mTextures));
        mPlayerAircraft = leader.get();
        mPlayerAircraft->setPosition(mSpawnPosition);
        mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
        mSceneLayers[(int)Layer::Air]->addChild(std::move(leader));

        // rightEscort
        std::unique_ptr<Aircraft> rightEscort(
            new Aircraft(Aircraft_t::Raptor, mTextures));
        rightEscort->setPosition({80.f, 50.f});
        mPlayerAircraft->addChild(std::move(rightEscort));
        // leftEscort
        std::unique_ptr<Aircraft> leftEscort(
            new Aircraft(Aircraft_t::Raptor, mTextures));
        leftEscort->setPosition({-80.f, 50.f});
        mPlayerAircraft->addChild(std::move(leftEscort));
    };

    void adoptPlayerVelocity() {
        sf::Vector2f vel = mPlayerAircraft->getVelocity();
        if (vel.x != 0.f && vel.y != 0.f) {
            mPlayerAircraft->setVelocity(
                vel / std::sqrt(2.f)); // fix diagonal velocity
        }
        mPlayerAircraft->accelerate(0.f, mScrollSpeed);
    }

    void adoptPlayerPosition() {
        // Keep player's position inside the screen bounds, at least
        // borderDistance units from the border
        sf::FloatRect viewBounds(mWorldView.getCenter() -
                                     mWorldView.getSize() / 2.f,
                                 mWorldView.getSize());
        const float borderDistance = 40.f;

        sf::Vector2f position      = mPlayerAircraft->getPosition();
        position.x =
            std::max(position.x, viewBounds.position.x + borderDistance);
        position.x =
            std::min(position.x, viewBounds.position.x + viewBounds.size.x -
                                     borderDistance);
        position.y =
            std::max(position.y, viewBounds.position.y + borderDistance);
        position.y =
            std::min(position.y, viewBounds.position.y + viewBounds.size.y -
                                     borderDistance);
        mPlayerAircraft->setPosition(position);
    }

  private:
    sf::RenderWindow &mWindow;
    sf::View mWorldView;
    TextureHolder mTextures;
    SceneNode mSceneGraph;
    std::array<SceneNode *, (size_t)Layer::LayerCount> mSceneLayers;
    sf::FloatRect mWorldBounds;
    sf::Vector2f mSpawnPosition;
    float mScrollSpeed;
    Aircraft *mPlayerAircraft;
    CommandQueue mCommandQueue;
};
