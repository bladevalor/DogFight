#include "Game/Application.hpp"

#include <SFML/System.hpp>
#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

int main() {

    try {
        Application app;
        app.run();
    } catch (std::exception &e) {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }
}
