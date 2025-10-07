#pragma once

#include "category.hpp"
#include <SFML/System/Time.hpp>
#include <cassert>
#include <functional>

class SceneNode;

struct Command {
    Command() : action(), category(GameObjectCategory::None) {};
    std::function<void(SceneNode &, sf::Time)> action;
    // unsigned int category;
    GameObjectCategory category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode &, sf::Time)> derivedAction(Function fn) {
    return [=](SceneNode &node, sf::Time dt) {
        // check if "down-cast" is safe
        assert(dynamic_cast<GameObject *>(&node) != nullptr);

        // down-cast and invoke function
        fn(static_cast<GameObject &>(node), dt);
    };
}
