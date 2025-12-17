#include "Engine/SceneNode.hpp"
#include "Engine/Utility.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

float distance(const SceneNode &lhs, const SceneNode &rhs) {
    return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

bool collision(SceneNode &lhs, SceneNode &rhs) {
    bool isColliding =
        lhs.getBoundingBox().findIntersection(rhs.getBoundingBox()).has_value();
    if (isColliding) {
        return true;
    }
    return false;
}

SceneNode::SceneNode(GameObjectCategory category)
    : mChildren(), mParent(nullptr), mDefaultCategory(category) {};

void SceneNode::addChild(SceneNodePointer child) {
    child->mParent = this;
    mChildren.push_back(std::move(child));
}
SceneNode::SceneNodePointer SceneNode::removeChild(const SceneNode &node) {
    auto found = std::find_if(
        mChildren.begin(), mChildren.end(),
        [&](SceneNodePointer &p) -> bool { return p.get() == &node; });

    assert(found != mChildren.end());

    SceneNodePointer result = std::move(*found);
    result->mParent         = nullptr;
    mChildren.erase(found);
    return result;
}
void SceneNode::update(sf::Time dt, CommandQueue &commands) {
    updateCurrent(dt, commands);
    updateChild(dt, commands);
};

sf::Transform SceneNode::getWorldTransform() const {
    sf::Transform transform = sf::Transform::Identity;
    for (const SceneNode *node = this; node != nullptr; node = node->mParent) {
        transform = node->getTransform() * transform;
    }
    return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const {
    return getWorldTransform().transformPoint(sf::Vector2f());
}

GameObjectCategory SceneNode::getCategory() const { return mDefaultCategory; }

void SceneNode::onCommand(Command command, sf::Time dt) {
    if ((int)(command.category & getCategory()) != 0) {
        command.action(*this, dt);
    }

    for (SceneNodePointer &child : mChildren) {
        child->onCommand(command, dt);
    }
}

bool SceneNode::isDestroyed() const {
    return false; // INFO: by defaulst scene nodes dont have to be removed
}

void SceneNode::checkNodeCollision(SceneNode &node,
                                   std::set<CollisionPair> &collisionMatches) {
    if (this != &node && collision(*this, node) && !isDestroyed() &&
        !node.isDestroyed()) {
        collisionMatches.insert(std::minmax(this, &node));
    }

    for (auto &child : mChildren) {
        child->checkNodeCollision(node, collisionMatches);
    }
}

void SceneNode::checkSceneCollision(SceneNode &sceneGraph,
                                    std::set<CollisionPair> &collisionMatches) {
    checkNodeCollision(sceneGraph, collisionMatches);

    for (auto &child : sceneGraph.mChildren) {
        checkSceneCollision(*child, collisionMatches);
    }
}

bool SceneNode::isMarkedForRemoval() const { return isDestroyed(); }

void SceneNode::removeWrecks() {
    std::erase_if(mChildren, [&](const SceneNodePointer &node) -> bool {
        return node->isMarkedForRemoval();
    });

    for (auto &node : mChildren) {
        node->removeWrecks();
    }
}

sf::FloatRect SceneNode::getBoundingBox() const {
    return getWorldTransform().transformRect(sf::FloatRect());
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform.combine(getTransform());

    drawCurrent(target, states);
    drawChildren(target, states);

    // show boundingBox
    drawBoundingBox(target, states);
};

void SceneNode::drawCurrent(sf::RenderTarget &target,
                            sf::RenderStates states) const {
    // Do nothing
};

void SceneNode::drawChildren(sf::RenderTarget &target,
                             sf::RenderStates states) const {
    for (const SceneNodePointer &child : mChildren) {
        child->draw(target, states);
    }
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue &commands) {
    // Do nothing
};

void SceneNode::updateChild(sf::Time dt, CommandQueue &commands) {
    for (const SceneNodePointer &child : mChildren) {
        child->update(dt, commands);
    }
};

void SceneNode::drawBoundingBox(sf::RenderTarget &target,
                                sf::RenderStates states) const {
    sf::FloatRect rect = getBoundingBox();
    sf::RectangleShape shape;

    shape.setPosition(rect.position);
    shape.setSize(rect.size);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Red);

    target.draw(shape);
}
