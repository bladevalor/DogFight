#pragma once

#include "Command.hpp"
#include "Engine/CommandQueue.hpp"
#include "category.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <set>
#include <utility>

class SceneNode : public sf::Transformable, public sf::Drawable {
  public:
    typedef std::unique_ptr<SceneNode> SceneNodePointer;
    typedef std::pair<SceneNode *, SceneNode *> CollisionPair;

  public:
    explicit SceneNode(GameObjectCategory category = GameObjectCategory::None);
    SceneNode(const SceneNode &) = delete; // disable copy constructor
    SceneNode &operator=(const SceneNode &) = delete; // disable copy assignment

    void addChild(SceneNodePointer child);
    SceneNodePointer removeChild(const SceneNode &node);

    void update(sf::Time dt, CommandQueue &commands);

    sf::Transform getWorldTransform() const;
    sf::Vector2f getWorldPosition() const;

    virtual GameObjectCategory getCategory() const;
    void onCommand(Command command, sf::Time dt);

    void checkNodeCollision(SceneNode &node,
                            std::set<CollisionPair> &collisionMatches);
    void checkSceneCollision(SceneNode &sceneGraph,
                             std::set<CollisionPair> &collisionMatches);

    virtual bool isDestroyed() const;
    virtual bool isMarkedForRemoval() const;
    void removeWrecks();
    sf::FloatRect getBoundingBox() const;

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawBoundingBox(sf::RenderTarget &target,
                         sf::RenderStates state) const;

    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    void updateChild(sf::Time dt, CommandQueue &commands);

  private:
    std::vector<SceneNodePointer> mChildren;
    SceneNode *mParent;
    GameObjectCategory mDefaultCategory;
};

float distance(const SceneNode &lhs, const SceneNode &rhs);
bool collision(SceneNode &lhs, SceneNode &rhs);
