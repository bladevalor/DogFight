#pragma once

#include "GUI/Component.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <vector>

namespace GUI {
class Container : public Component {
  public:
    typedef std::shared_ptr<Container> ContainerPointer;

  public:
    Container() : mChildren(), mSelectedChild(-1) {}

    void pack(Component::ComponentPointer component) {
        mChildren.push_back(component);

        if (!hasSelection() && component->isSelectable()) {
            select(mChildren.size() - 1);
        }
    }

    virtual bool isSelectable() const { return false; }

    virtual void handleEvent(const sf::Event &event) {
        if (hasSelection() && mChildren[mSelectedChild]->isActive()) {
            mChildren[mSelectedChild]->handleEvent(event);
        } else if (auto *key = event.getIf<sf::Event::KeyReleased>()) {
            if (key->code == sf::Keyboard::Key::W ||
                key->code == sf::Keyboard::Key::Up) {
                selectPrevious();
            } else if (key->code == sf::Keyboard::Key::S ||
                       key->code == sf::Keyboard::Key::Down) {
                selectNext();
            } else if (key->code == sf::Keyboard::Key::Enter ||
                       key->code == sf::Keyboard::Key::Space) {
                mChildren[mSelectedChild]->activate();
            }
        }
    }

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform = getTransform();
        for (auto &child : mChildren) {
            target.draw(*child, states);
        }
    }

    bool hasSelection() const { return mSelectedChild >= 0; }

    void select(std::size_t index) {
        if (mChildren[index]->isSelectable()) {
            if (hasSelection()) {
                mChildren[mSelectedChild]->deselect();
            }

            mChildren[index]->select();
            mSelectedChild = index;
        }
    }

    void selectNext() {
        if (!hasSelection()) {
            return;
        }

        int next = mSelectedChild;
        do {
            next = (next + 1) % mChildren.size();
        } while (!mChildren[next]->isSelectable());
        select(next);
    }

    void selectPrevious() {
        if (!hasSelection()) {
            return;
        }

        int previous = mSelectedChild;
        do {
            previous = (previous + mChildren.size() - 1) % mChildren.size();
        } while (!mChildren[previous]->isSelectable());
        select(previous);
    }

  private:
    std::vector<Component::ComponentPointer> mChildren;
    int mSelectedChild;
};
} // namespace GUI
