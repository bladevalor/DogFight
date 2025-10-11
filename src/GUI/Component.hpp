#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace GUI {
class Component : public sf::Drawable, public sf::Transformable {
  public:
    typedef std::shared_ptr<Component> ComponentPointer;

  public:
    Component() : mIsSelected(false), mIsActive(false) {}
    virtual ~Component() {}

    virtual bool isSelectable() const = 0;
    bool isSelected() const { return mIsSelected; }
    virtual void select() { mIsSelected = true; }
    virtual void deselect() { mIsSelected = false; }

    virtual bool isActive() const { return mIsActive; }
    virtual void activate() { mIsActive = true; }
    virtual void deactivate() { mIsActive = false; }

    virtual void handleEvent(const sf::Event &event) = 0;

  private:
    bool mIsSelected;
    bool mIsActive;
};
} // namespace GUI
