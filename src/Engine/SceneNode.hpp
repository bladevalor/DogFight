#pragma once

#include "Command.hpp"
#include "category.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Time.hpp>
#include <algorithm>

class SceneNode : public sf::Transformable, public sf::Drawable {
  public:
    typedef std::unique_ptr<SceneNode> SceneNodePointer;

  public:
    SceneNode() : mChildren(), mParent(nullptr) {};
    SceneNode(const SceneNode &) = delete; // disable copy constructor
    SceneNode &operator=(const SceneNode &) = delete; // disable copy assignment

    void addChild(SceneNodePointer child) {
        child->mParent = this;
        mChildren.push_back(std::move(child));
    }

    SceneNodePointer removeChild(const SceneNode &node) {
        auto found = std::find_if(
            mChildren.begin(), mChildren.end(),
            [&](SceneNodePointer &p) -> bool { return p.get() == &node; });

        assert(found != mChildren.end());

        SceneNodePointer result = std::move(*found);
        result->mParent         = nullptr;
        mChildren.erase(found);
        return result;
    }
    void update(sf::Time dt) {
        updateCurrent(dt);
        updateChild(dt);
    };

    sf::Transform getWorldTransform() const {
        sf::Transform transform = sf::Transform::Identity;
        for (const SceneNode *node = this; node != nullptr;
             node                  = node->mParent) {
            transform = node->getTransform() * transform;
        }
        return transform;
    }

    sf::Vector2f getWorldPosition() const {
        return getWorldTransform() * this->getPosition();
    }

    virtual GameObjectCategory getCategory() const {
        return GameObjectCategory::Scene;
    }

    void onCommand(Command command, sf::Time dt) {
        if (command.category == getCategory()) {
            command.action(*this, dt);
        }

        for (SceneNodePointer &child : mChildren) {
            child->onCommand(command, dt);
        }
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        drawCurrent(target, states);
        drawChildren(target, states);
    };

    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) const {
        // Do nothing
    };

    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const {
        for (const SceneNodePointer &child : mChildren) {
            child->draw(target, states);
        }
    }

    virtual void updateCurrent(sf::Time dt) {
        // Do nothing
    };
    void updateChild(sf::Time dt) {
        for (const SceneNodePointer &child : mChildren) {
            child->update(dt);
        }
    };

  private:
    std::vector<SceneNodePointer> mChildren;
    SceneNode *mParent;
};
