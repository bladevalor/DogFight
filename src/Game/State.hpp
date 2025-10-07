#pragma once

#include "Engine/Player.hpp"
#include "Engine/Resource.hpp"
#include "Game/StateIDs.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

// namespace sf {
// class RenderWindow;
// }
//
class StateStack;

class State {
  public:
    typedef std::unique_ptr<State> StatePointer;

    struct Context {
        Context(sf::RenderWindow &window, TextureHolder &textures,
                FontHolder &fonts, Player &player);

        sf::RenderWindow *window;
        TextureHolder *textures;
        FontHolder *fonts;
        Player *player;
    };

  public:
    State(StateStack &stack, Context context);
    virtual ~State();

    virtual void draw()                              = 0;
    virtual bool update(sf::Time dt)                 = 0;
    virtual bool handleEvent(const sf::Event &event) = 0;

  protected:
    void requestStackPush(StateID id);
    void requestStackPop();
    void requestStackClear();

    Context getContext() const;

  private:
    StateStack *mStack;
    Context mContext;
};
